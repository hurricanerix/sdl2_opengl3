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
#include "3dmath.h"

#include "logger.h"


void log_matrix(float *mat, int rowc, int colc)
{
    assert(mat != NULL);
    log_debug("log_matrix {");
    log_debug("  -in- mat - %x", mat);
    log_debug("  -in- rowc - %d", rowc);
    log_debug("  -in- colc - %d", colc);

    for (int i = 0; i < rowc; i++) {
        for (int j = 0; j < colc; j++) {
            log_debug("mat[%d][%d] - %f", i, j, mat[rowc * i + colc]);
        }
    }

    log_debug("log_matrix }");
}


void get_rot_matrix(float *m, float x, float y, float z)
{
    assert(m != NULL);
    log_debug("get_rot_matrix {");
    log_debug("  -in- m - %x", m);
    log_debug("  -in- x - %f", x);
    log_debug("  -in- y - %f", y);
    log_debug("  -in- z - %f", z);

    float c1 = cos(x);
    float c2 = cos(y);
    float c3 = cos(z);
    float s1 = sin(x);
    float s2 = sin(y);
    float s3 = sin(z);

    m[0] = c2 * c3;
    m[1] = -c2 * s3;
    m[2] = s2;
    m[3] = (c1 * s3) + (c3 * s1 * s2);
    m[4] = (c1 * c3) - (s1 * s2 * s3);
    m[5] = -c2 * s1;
    m[6] = (s1 * s3) - (c1 * c3 * s2);
    m[7] = (c3 * s1) + (c1 * s2 * s3);
    m[8] = c1 * c2;

    log_matrix(m, 3, 3);
    log_debug("get_rot_matrix {");
}

// ----------------------------------------------------
// VECTOR STUFF
//

// res = a cross b;
void crossProduct(float *a, float *b, float *res)
{
    assert(a != NULL);
    assert(b != NULL);
    assert(res != NULL);
    log_debug("crossProduct {");
    log_debug("  -in- a - %x", a);
    log_debug("  -in- b - %x", b);
    log_debug("  -in- res - %x", res);

    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];

    log_debug("crossProduct }");
}

// Normalize a vec3
void normalize(float *a)
{
    assert(a != NULL);
    log_debug("normalize {");
    log_debug("  -in- a - %x", a);

    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);

    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;

    log_debug("normalize }");
}

// ----------------------------------------------------
// MATRIX STUFF
//

// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void setIdentityMatrix(float *mat, int size)
{
    assert(mat != NULL);
    log_debug("setIdentityMatrix {");
    log_debug("  -in- mat - %x", mat);
    log_debug("  -in- size - %d", size);

    // fill matrix with 0s
    for (int i = 0; i < size * size; ++i)
            mat[i] = 0.0f;

    // fill diagonal with 1s
    for (int i = 0; i < size; ++i)
        mat[i + i * size] = 1.0f;

    log_matrix(mat, size, size);
    log_debug("setIdentityMatrix }");
}

// a = b;
void copy_matrix(float *a, float *b)
{
    for (int i = 0; i < 16; i++) {
        a[i] = b[i];
    }
}

//
// a = a * b;
//
void multMatrix(float *a, float *b)
{
    assert(a != NULL);
    assert(b != NULL);
    log_debug("multMatrix {");
    log_debug("  -in- a - %x", a);
    log_debug("  -in- b - %x", b);

    float res[16];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
            }
        }
    }
    memcpy(a, res, 16 * sizeof(float));

    log_matrix(a, 4, 4);
    log_matrix(b, 4, 4);
    log_debug("multMatrix }");
}

// Defines a transformation matrix mat with a translation
void setTranslationMatrix(float *mat, float x, float y, float z)
{
    assert(mat != NULL);
    log_debug("setTranslationMatrix {");
    log_debug("  -in- mat - %x", mat);
    log_debug("  -in- x - %f", x);
    log_debug("  -in- y - %f", y);
    log_debug("  -in- z - %f", z);

    setIdentityMatrix(mat, 4);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;

    log_matrix(mat, 4, 4);
    log_debug("setTranslationMatrix }");
}

// ----------------------------------------------------
// Projection Matrix
//

void buildProjectionMatrix(float *projMatrix, float fov, float ratio,
    float nearP, float farP)
{
    assert(projMatrix != NULL);
    log_debug("buildProjectionMatrix {");
    log_debug("  -in- projMatrix - %x", projMatrix);
    log_debug("  -in- fov - %f", fov);
    log_debug("  -in- ratio - %f", ratio);
    log_debug("  -in- nearP - %f", nearP);
    log_debug("  -in- farP - %f", farP);

    float f = 1.0f / tan (fov * (M_PI / 360.0));

    setIdentityMatrix(projMatrix,4);

    projMatrix[0] = f / ratio;
    projMatrix[1 * 4 + 1] = f;
    projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    projMatrix[2 * 4 + 3] = -1.0f;
    projMatrix[3 * 4 + 3] = 0.0f;

    log_matrix(projMatrix, 4, 4);
    log_debug("buildProjectionMatrix }");
}

// ----------------------------------------------------
// View Matrix
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector (remmeber gluLookAt?)
//

void setCamera(float *viewMatrix, float posX, float posY, float posZ,
    float lookAtX, float lookAtY, float lookAtZ)
{
    assert(viewMatrix != NULL);
    log_debug("setCamera {");
    log_debug("  -in- viewMatrix - %x", viewMatrix);
    log_debug("  -in- posX - %f", posX);
    log_debug("  -in- posY - %f", posY);
    log_debug("  -in- posZ - %f", posZ);
    log_debug("  -in- lookAtX - %f", lookAtX);
    log_debug("  -in- lookAtY - %f", lookAtY);
    log_debug("  -in- lookAtZ - %f", lookAtZ);

    float dir[3], right[3], up[3];

    up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;

    dir[0] =  (lookAtX - posX);
    dir[1] =  (lookAtY - posY);
    dir[2] =  (lookAtZ - posZ);
    normalize(dir);

    crossProduct(dir,up,right);
    normalize(right);

    crossProduct(right,dir,up);
    normalize(up);

    float aux[16];

    viewMatrix[0]  = right[0];
    viewMatrix[4]  = right[1];
    viewMatrix[8]  = right[2];
    viewMatrix[12] = 0.0f;

    viewMatrix[1]  = up[0];
    viewMatrix[5]  = up[1];
    viewMatrix[9]  = up[2];
    viewMatrix[13] = 0.0f;

    viewMatrix[2]  = -dir[0];
    viewMatrix[6]  = -dir[1];
    viewMatrix[10] = -dir[2];
    viewMatrix[14] =  0.0f;

    viewMatrix[3]  = 0.0f;
    viewMatrix[7]  = 0.0f;
    viewMatrix[11] = 0.0f;
    viewMatrix[15] = 1.0f;

    setTranslationMatrix(aux, -posX, -posY, -posZ);

    multMatrix(viewMatrix, aux);

    log_matrix(viewMatrix, 4, 4);
    log_debug("setCamera }");
}
