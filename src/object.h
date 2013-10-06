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
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "config.h"
#include "status.h"
#include "shader.h"
#include "texture.h"

#define OBJECT_GL_BUFFER_COUNT (5)
#define OBJECT_MAX_TEXTURE_COUNT (2)

typedef struct Vertex {
  float x, y, z;             /* the usual 3-space position of a vertex */
  float nx, ny, nz;
  float s, t;
} Vertex;

typedef struct Face {
  unsigned char intensity; /* this user attaches intensity to faces */
  unsigned char nverts;    /* number of vertex indices in list */
  int *verts;              /* vertex index list */
} Face;

typedef struct Object {
    Status status;
    char *filename;
    unsigned int vertex_count;
    unsigned int triangle_count;
    vec3 *vertices;
    vec3 *normals;
    vec3 *tangents;
    vec2 *tex_coords;
    ivec3 *triangles;
    Shader *shader;
    Texture *colormap;
    Texture *normalmap;
    GLuint gl_buffers[OBJECT_GL_BUFFER_COUNT];
} Object;

Object init_object();

void load_object(char *filename, Shader *shader, Texture *colormap,
        Texture *normalmap, Object *o);

void bind_object(Object *o);

void render_object(Object *o);

void unbind_object(Object *o);

void destroy_object(Object *o);

#endif//__OBJECT_H__
