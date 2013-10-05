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
#ifndef __3D_MATH_H__
#define __3D_MATH_H__

typedef struct vec2 {
    union {float x; float s;};
    union {float y; float t;};
} vec2;

typedef struct vec3 {
    union {float x; float r; float s;};
    union {float y; float g; float t;};
    union {float z; float b;};
} vec3;

typedef struct vec4 {
    union {float x; float r; float s;};
    union {float y; float g; float t;};
    union {float z; float b;};
    union {float w; float a;};
} vec4;

typedef struct mat3 {
    vec3 row1;
    vec3 row2;
    vec3 row3;
} mat3;

typedef struct mat4 {
    vec4 row1;
    vec4 row2;
    vec4 row3;
    vec4 row4;
} mat4;

void print_vec2(char *label, vec2 v);
void print_vec3(char *label, vec3 v);
void print_vec4(char *label, vec4 v);
void print_mat3(char *label, mat3 m);
void print_mat4(char *label, mat4 m);

mat3 get_rotation_matrix(vec3 rotation);

vec3 get_surface_local_tangent(
        vec3 p1, vec2 t1, vec3 p2, vec2 t2, vec3 p3, vec2 t3);

mat4 get_identity_mat4();
mat4 mult_mat4(mat4 a, mat4 b);
mat4 get_translation_mat4(vec3 v);
mat4 get_view_matrix(vec3 pos, vec3 lookAt);
mat4 get_projection_matrix(float fov, float ratio,
    float nearP, float farP);
mat3 get_rotation_matrix(vec3 dir);

#endif//__3D_MATH_H__
