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
#ifndef __SHADER_H__
#define __SHADER_H__

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include "config.h"
#include "status.h"

typedef struct Shader {
    Status status;
    unsigned int ref_count;
    GLuint vertex_loc;
    GLuint normal_loc;
    GLuint tangent_loc;
    GLuint tex_coords_loc;
    GLuint proj_matrix_loc;
    GLuint view_matrix_loc;
    GLuint rot_matrix_loc;
    char vert_filename[MAX_FILENAME_LEN];
    char frag_filename[MAX_FILENAME_LEN];
    GLuint program_id;
    GLuint vert_program_id;
    GLuint frag_program_id;
} Shader;

#define print_opengl_error() print_gl_error(__FILE__, __LINE__)

void init_shader(Shader *s);

void load_shader(Shader *s, char *vert_file, char *frag_file);

void bind_uniform(Shader *s, UniformConfig *config);

void destroy_shader(Shader *s);

void print_program_log(GLuint obj);

void print_shader_log(GLuint obj);

void _print_shader(FILE *fp, Shader *s);
#define print_shader(s) (_print_shader(stderr, (s)))

#endif// __SHADER_H__
