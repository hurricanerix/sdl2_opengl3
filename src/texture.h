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
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include "status.h"

typedef struct Texture {
    Status status;
    unsigned int ref_count;
    GLuint texture_id;
    char *filename;
    unsigned int width;
    unsigned int height;
    unsigned char *data;
} Texture;

/// Init texture.
Texture init_texture();

/// Load the BMP data from filename and store it in t.
void load_texture(char *filename, Texture *t);

/// Bind the texture to opengl.
void bind_texture(Texture *t);

/// Unbind the texture to opengl.
void unbind_texture(Texture *t);

/// Free memory and rest variables.
void destroy_texture(Texture *t);

#endif//__TEXTURE_H__
