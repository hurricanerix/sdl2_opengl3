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

void print_matrix(char *label, float *mat, int size);
void get_rot_matrix(float *m, float x, float y, float z);

void copy_matrix(float *a, float *b);
void crossProduct( float *a, float *b, float *res);
void normalize(float *a);
void setIdentityMatrix( float *mat, int size);
void multMatrix(float *a, float *b);
void setTranslationMatrix(float *mat, float x, float y, float z);
void buildProjectionMatrix(float *projMatrix, float fov, float ratio, float nearP, float farP);
void setCamera(float *viewMatrix, float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ);
void get_sl_tangent(
    float pAx, float pAy, float pAz,
    float tAx, float tAy,
    float pBx, float pBy, float pBz,
    float tBx, float tBy,
    float pCx, float pCy, float pCz,
    float tCx, float tCy,
    float *tx, float *ty, float *tz);

#endif//__3D_MATH_H__
