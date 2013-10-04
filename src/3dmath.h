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

void print_mat4(char *label, mat4 m);
void get_rot_matrix(float *m, float x, float y, float z);

void get_sl_tangent(
    float pAx, float pAy, float pAz,
    float tAx, float tAy,
    float pBx, float pBy, float pBz,
    float tBx, float tBy,
    float pCx, float pCy, float pCz,
    float tCx, float tCy,
    float *tx, float *ty, float *tz);

mat4 get_identity_mat4();
mat4 mult_mat4(mat4 a, mat4 b);
mat4 get_translation_mat4(float x, float y, float z);
mat4 get_view_matrix(float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ); // setCamera replacement
mat4 get_projection_matrix(float fov, float ratio,
    float nearP, float farP);

#endif//__3D_MATH_H__
