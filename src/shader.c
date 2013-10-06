/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Richard Hawkins
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include <string.h>

#include "config.h"
#include "logger.h"
#include "main.h"
#include "text.h"
#include "shader.h"
#include "status.h"


#define SHADER_DEFAULT_REF_COUNT (0)
#define SHADER_DEFAULT_VERTEX_LOC (0)
#define SHADER_DEFAULT_NORMAL_LOC (0)
#define SHADER_DEFAULT_TANGENT_LOC (0)
#define SHADER_DEFAULT_TEX_COORDS_LOC (0)
#define SHADER_DEFAULT_PROJ_MATRIX_LOC (0)
#define SHADER_DEFAULT_VIEW_MATRIX_LOC (0)
#define SHADER_DEFAULT_ROTATION_MATRIX_LOC (0)
#define SHADER_DEFAULT_VERT_FILENAME (NULL)
#define SHADER_DEFAULT_FRAG_FILENAME (NULL)
#define SHADER_DEFAULT_PROGRAM_ID (0)
#define SHADER_DEFAULT_VERT_PROGRAM_ID (0)
#define SHADER_DEFAULT_FRAG_PROGRAM_ID (0)


void _set_shader_defaults(Shader *s);


void init_shader(Shader *s)
{
    assert(s != NULL);

    init_status(&s->status);
    fprintf(stderr, "TEST %d\n", s->status.is_error);

    _set_shader_defaults(s);
}

void load_shader(char *vert_filename, char *frag_filename, Shader *s)
{
    assert(s != NULL);
    assert(vert_filename != NULL);
    assert(frag_filename != NULL);

    s->vert_filename = malloc(strnlen(vert_filename, MAX_FILENAME_LEN));
    if (s->vert_filename == NULL) {
        destroy_shader(s);
        s->status.is_error = TRUE;
        set_error_msg(s->status, "Could not malloc vert_file");
        return;
    }
    strncpy(s->vert_filename, (vert_filename), MAX_FILENAME_LEN);
    fprintf(stderr, "NAME: %s\n", vert_filename);

    s->frag_filename = malloc(strnlen(frag_filename, MAX_FILENAME_LEN));
    if (s->frag_filename == NULL) {
        destroy_shader(s);
        s->status.is_error = TRUE;
        set_error_msg(s->status, "Could not malloc frag_file");
        return;
    }
    strncpy(s->frag_filename, (frag_filename), MAX_FILENAME_LEN);

    char *vs = NULL, *fs = NULL;

    s->vert_program_id = glCreateShader(GL_VERTEX_SHADER);
    s->frag_program_id = glCreateShader(GL_FRAGMENT_SHADER);

    int vs_size;
    vs = text_file_read(s->vert_filename, &vs_size);
    int fs_size;
    fs = text_file_read(s->frag_filename, &fs_size);

    const char * vv = vs;
    const char * ff = fs;

    glShaderSource(s->vert_program_id, 1, &vv, NULL);
    glShaderSource(s->frag_program_id, 1, &ff, NULL);

    free(vs); free(fs);

    glCompileShader(s->vert_program_id);
    glCompileShader(s->frag_program_id);

    print_shader_log(s->vert_program_id);
    print_shader_log(s->frag_program_id);

    s->program_id = glCreateProgram();
    glAttachShader(s->program_id, s->vert_program_id);
    glAttachShader(s->program_id, s->frag_program_id);

    glBindFragDataLocation(s->program_id, 0, "FracColor");
    glLinkProgram(s->program_id);
    print_program_log(s->program_id);

    s->vertex_loc = glGetAttribLocation(s->program_id, "MCvertex");
    s->normal_loc = glGetAttribLocation(s->program_id, "MCnormal");
    s->tangent_loc = glGetAttribLocation(s->program_id, "MCtangent");
    s->tex_coords_loc = glGetAttribLocation(s->program_id, "TexCoord0");

    s->proj_matrix_loc = glGetUniformLocation(s->program_id, "MVPMatrix");
    s->view_matrix_loc = glGetUniformLocation(s->program_id, "MVMatrix");
    s->rot_matrix_loc = glGetUniformLocation(s->program_id, "RotMatrix");
}

void bind_uniform(Shader *s, UniformConfig *config)
{
    assert(config != NULL);

    GLuint loc = glGetUniformLocation(s->program_id, config->name);
    switch(config->type) {
    case UNIFORM_INT:
        glUniform1i(loc, config->i);
        break;
    case UNIFORM_FLOAT:
        glUniform1f(loc, config->x);
        break;
    case UNIFORM_VEC2:
        glUniform2f(loc, config->x, config->y);
        break;
    case UNIFORM_VEC3:
        glUniform3f(loc, config->x, config->y, config->z);
        break;
    case UNIFORM_VEC4:
        glUniform4f(loc, config->x, config->y, config->z, config->w);
        break;
    default:
        break;
    }
}

int print_gl_error(char *file, int line)
{
    assert(file != NULL);
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        //printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        printf("glError in file %s @ line %d: \n", file, line);
        retCode = 1;
        glErr = glGetError();
    }

    return retCode;
}

void print_shader_log(GLuint obj)
{

    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0) {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }

}

void print_program_log(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }

}

void destroy_shader(Shader *s)
{
    if (s == NULL) {
        return;
    }

    if (s->vert_program_id != SHADER_DEFAULT_VERT_PROGRAM_ID) {
        glDeleteShader(s->vert_program_id);
    }

    if (s->frag_program_id != SHADER_DEFAULT_FRAG_PROGRAM_ID) {
        glDeleteShader(s->frag_program_id);
    }

    if (s->program_id != SHADER_DEFAULT_PROGRAM_ID) {
        glDeleteProgram(s->program_id);
    }

    free(s->vert_filename);
    free(s->frag_filename);

    _set_shader_defaults(s);
}

void _set_shader_defaults(Shader *s)
{
    assert(s != NULL);

    s->ref_count = SHADER_DEFAULT_REF_COUNT;
    s->vertex_loc = SHADER_DEFAULT_VERTEX_LOC;
    s->normal_loc = SHADER_DEFAULT_NORMAL_LOC;
    s->tangent_loc = SHADER_DEFAULT_TANGENT_LOC;
    s->tex_coords_loc = SHADER_DEFAULT_TEX_COORDS_LOC;
    s->proj_matrix_loc = SHADER_DEFAULT_PROJ_MATRIX_LOC;
    s->view_matrix_loc = SHADER_DEFAULT_VIEW_MATRIX_LOC;
    s->rot_matrix_loc = SHADER_DEFAULT_ROTATION_MATRIX_LOC;
    s->vert_filename = SHADER_DEFAULT_VERT_FILENAME;
    s->frag_filename = SHADER_DEFAULT_FRAG_FILENAME;
    s->program_id = SHADER_DEFAULT_PROGRAM_ID;
    s->vert_program_id = SHADER_DEFAULT_VERT_PROGRAM_ID;
    s->frag_program_id = SHADER_DEFAULT_FRAG_PROGRAM_ID;
}
