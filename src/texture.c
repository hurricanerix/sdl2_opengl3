/*
 * the mit license (mit)
 *
 * copyright (c) 2013 richard hawkins
 *
 * permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "software"), to deal
 * in the software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the software, and to permit persons to whom the software is
 * furnished to do so, subject to the following conditions:
 *
 * the above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the software.
 *
 * the software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. in no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in
 * the software.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include "logger.h"
#include "main.h"
#include "status.h"
#include "texture.h"


#define TEXTURE_DEFAULT_REF_COUNT (0)
#define TEXTURE_DEFAULT_TEXTURE_ID (0)
#define TEXTURE_DEFAULT_WIDTH (0)
#define TEXTURE_DEFAULT_HEIGHT (0)
#define TEXTURE_DEFAULT_FILENAME (NULL)
#define TEXTURE_DEFAULT_DATA (NULL)


void _set_texture_defaults(Texture *t);


Texture init_texture()
{
    Texture t;
    init_status(&t.status);

    _set_texture_defaults(&t);

    return t;
}

void load_texture(char *filename, Texture *t)
{
    assert(t != NULL);
    assert(filename != NULL);

    t->status.is_error = FALSE;

    if (t->filename != NULL &&
            strncmp(t->filename, filename, MAX_FILENAME_LEN) != 0) {
        destroy_texture(t);
    }

    t->filename = malloc(strnlen((filename), MAX_FILENAME_LEN));
    if (!t->filename) {
        t->status.is_error = TRUE;
        set_error_msg(
            t->status, "Could not allocate memory for texture filename.");
        return;
    }
    strncpy(t->filename, (filename), MAX_FILENAME_LEN);

    FILE *file = fopen(t->filename, "rb");
    if (!file) {
        t->status.is_error = TRUE;
        set_error_msg(t->status, "BMP could not be opened");
        return;
    }

    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    if (fread(header, 1, 54, file)!=54){ // If not 54 bytes read : problem
        t->status.is_error = TRUE;
        set_error_msg(t->status, "Could not read BMP header");
        fclose(file);
        return;
    }

    if (header[0] != 'B' || header[1] != 'M'){
        t->status.is_error = TRUE;
        set_error_msg(t->status, "Not a correct BMP file");
        fclose(file);
        return;
    }

    // Position in the file where the actual data begins
    unsigned int data_pos = *(int*)&(header[0x0A]);
    unsigned int image_size  = *(int*)&(header[0x22]);
    t->width = *(int*)&(header[0x12]);
    t->height = *(int*)&(header[0x16]);

    if (image_size == 0) {
        // Some BMP files are misformatted, guess missing information
        image_size=t->width * t->height * 3;
    }

    if (data_pos == 0) {
        // The BMP header is done that way
        data_pos = 54;
    }

    // Create a buffer
    t->data = malloc(sizeof(unsigned char) * image_size);
    if (t->data == NULL) {
        t->status.is_error = TRUE;
        set_error_msg(t->status, "Not allocate storage for BMP.");
        fclose(file);
        return;
    }

    // Read the actual data from the file into the buffer
    if (fread(t->data, 1, image_size, file) == 0) {
        t->status.is_error = TRUE;
        set_error_msg(t->status, "Could not read BMP data.");
        fclose(file);
        return;
    }

    //Everything is in memory now, the file can be closed
    fclose(file);
}

void bind_texture(Texture *t)
{
    glGenTextures(1, &t->texture_id);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t->texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->width, t->height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, t->data);

    //free(t->data);
}

void unbind_texture(Texture *t)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void destroy_texture(Texture *t)
{
    if (t == NULL) {
        return;
    }

    unbind_texture(t);

    free(t->filename);
    free(t->data);

    _set_texture_defaults(t);
}

void _set_texture_defaults(Texture *t)
{
    assert(t != NULL);

    t->ref_count = TEXTURE_DEFAULT_REF_COUNT;
    t->texture_id = TEXTURE_DEFAULT_TEXTURE_ID;
    t->width = TEXTURE_DEFAULT_WIDTH;
    t->height = TEXTURE_DEFAULT_HEIGHT;
    t->filename = TEXTURE_DEFAULT_FILENAME;
    t->data = TEXTURE_DEFAULT_DATA;
}
