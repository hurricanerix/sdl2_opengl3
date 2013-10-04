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
#include <math.h>
#include <memory.h>
#include <stdio.h>

#include "3dmath.h"
#include "logger.h"


void print_vec3(char *label, vec3 v)
{
    if (label != NULL) {
        printf("vec3 %s\n", label);
    }

    printf("%f,\t%f,\t%f\n", v.x, v.y, v.z);
}

void print_vec4(char *label, vec4 v)
{
    if (label != NULL) {
        printf("vec4 %s\n", label);
    }

    printf("%f,\t%f,\t%f,\t%f\n", v.x, v.y, v.z, v.w);
}

void print_mat4(char *label, mat4 m)
{
    if (label != NULL) {
        printf("mat4 %s\n", label);
    }

    printf("[\n");
    print_vec4(NULL, m.row1);
    print_vec4(NULL, m.row2);
    print_vec4(NULL, m.row3);
    print_vec4(NULL, m.row4);
    printf("]\n");
}

void print_matrix(char *label, float *mat, int size)
{
    assert(mat != NULL);

    printf("%s\n", label);

    for (int i = 0; i < size; i++) {
        printf("[");
        for (int j = 0; j < size; j++) {
            printf("%f, ", mat[size * i + size]);
        }
        printf("]\n");
    }
}

void get_rot_matrix(float *m, float x, float y, float z)
{
    assert(m != NULL);

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

    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];

}

vec3 cross_product(vec3 a, vec3 b)
{
    vec3 res;

    res.x = a.y * b.z  -  b.y * a.z;
    res.y = a.z * b.x  -  b.z * a.x;
    res.z = a.x * b.y  -  b.x * a.y;

    return res;
}

// Normalize a vec3
void normalize(float *a)
{
    assert(a != NULL);

    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);

    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}

vec3 normalize2(vec3 m)
{
    float mag = sqrt(m.x * m.x  +  m.y * m.y  +  m.z * m.z);

    if (mag == 0) {
        m.x = 0.0;
        m.y = 0.0;
        m.z = 0.0;
        return m;
    }

    m.x /= mag;
    m.y /= mag;
    m.z /= mag;

    return m;
}

// ----------------------------------------------------
// MATRIX STUFF
//

// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void setIdentityMatrix(float *mat, int size)
{
    assert(mat != NULL);

    // fill matrix with 0s
    for (int i = 0; i < size * size; ++i)
            mat[i] = 0.0f;

    // fill diagonal with 1s
    for (int i = 0; i < size; ++i)
        mat[i + i * size] = 1.0f;
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
}

// Defines a transformation matrix mat with a translation
void setTranslationMatrix(float *mat, float x, float y, float z)
{
    assert(mat != NULL);

    setIdentityMatrix(mat, 4);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
}

// ----------------------------------------------------
// Projection Matrix
//

void buildProjectionMatrix(float *projMatrix, float fov, float ratio,
    float nearP, float farP)
{
    assert(projMatrix != NULL);

    float f = 1.0f / tan (fov * (M_PI / 360.0));

    setIdentityMatrix(projMatrix,4);

    projMatrix[0] = f / ratio;
    projMatrix[1 * 4 + 1] = f;
    projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    projMatrix[2 * 4 + 3] = -1.0f;
    projMatrix[3 * 4 + 3] = 0.0f;

    print_matrix("TEST", projMatrix, 4);
}

mat4 get_projection_matrix(float fov, float ratio,
    float nearP, float farP)
{
    float f = 1.0f / tan (fov * (M_PI / 360.0));

    mat4 proj_matrix = get_identity_mat4();

    proj_matrix.row1.x = f / ratio;
    proj_matrix.row2.y = f;
    proj_matrix.row3.z = (farP + nearP) / (nearP - farP);
    proj_matrix.row4.z = (2.0f * farP * nearP) / (nearP - farP);
    proj_matrix.row3.w = -1.0f;
    proj_matrix.row4.w = 0.0f;

    return proj_matrix;
}

mat4 get_identity_mat4()
{
    mat4 m = {.row1={{.x=1.0}, {.y=0.0}, {.z=0.0}, {.w=0.0}},
              .row2={{.x=0.0}, {.y=1.0}, {.z=0.0}, {.w=0.0}},
              .row3={{.x=0.0}, {.y=0.0}, {.z=1.0}, {.w=0.0}},
              .row4={{.x=0.0}, {.y=0.0}, {.z=0.0}, {.w=1.0}}};
    return m;
}

mat4 mult_mat4(mat4 a, mat4 b)
{
    mat4 res;
    float *pr = (float *)&res;
    float *pa = (float *)&a;
    float *pb = (float *)&b;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            pr[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                pr[j*4 + i] += pa[k*4 + i] * pb[j*4 + k];
            }
        }
    }
    memcpy(pa, pr, 16 * sizeof(float));

    return res;
}

mat4 get_translation_mat4(float x, float y, float z)
{
    mat4 m = get_identity_mat4();
    m.row4.x = x;
    m.row4.y = y;
    m.row4.z = z;

    return m;
}

// ----------------------------------------------------
// View Matrix
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector (remmeber gluLookAt?)
//

void print_float3(char *label, float *a)
{
    printf("float3 %s\n", label);
    for (int i = 0; i < 3; i++){
        printf("%f, ", a[i]);
    }
    printf("\n");
}

void setCamera(float *viewMatrix, float posX, float posY, float posZ,
    float lookAtX, float lookAtY, float lookAtZ)
{
    assert(viewMatrix != NULL);

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
}

mat4 get_view_matrix(float posX, float posY, float posZ,
    float lookAtX, float lookAtY, float lookAtZ)
{
    vec3 dir = {{.x=lookAtX - posX}, {.y=lookAtY - posY}, {.z=lookAtZ-posZ}};
    vec3 right = {{.x=0.0}, {.y=0.0}, {.z=0.0}};
    vec3 up = {{.x=0.0}, {.y=1.0}, {.z=0.0}};

    dir = normalize2(dir);

    right = cross_product(dir, up);
    right = normalize2(right);

    up = cross_product(right, dir);
    up = normalize2(up);

    mat4 view_matrix;
    view_matrix.row1.x = right.x;
    view_matrix.row2.x = right.y;
    view_matrix.row3.x = right.z;
    view_matrix.row4.x = 0.0f;

    view_matrix.row1.y = up.x;
    view_matrix.row2.y = up.y;
    view_matrix.row3.y = up.z;
    view_matrix.row3.y = 0.0f;

    view_matrix.row1.z = -dir.x;
    view_matrix.row2.z = -dir.y;
    view_matrix.row3.z = -dir.z;
    view_matrix.row4.z = 0.0f;

    view_matrix.row1.w = 0.0f;
    view_matrix.row2.w = 0.0f;
    view_matrix.row3.w = 0.0f;
    view_matrix.row4.w = 1.0f;

    mat4 aux = get_translation_mat4(-posX, -posY, -posZ);
    view_matrix = mult_mat4(view_matrix, aux);

    return view_matrix;
}

// Get Surface Local Tangent
void get_sl_tangent(
    float pAx, float pAy, float pAz,
    float tAx, float tAy,
    float pBx, float pBy, float pBz,
    float tBx, float tBy,
    float pCx, float pCy, float pCz,
    float tCx, float tCy,
    float *tx, float *ty, float *tz)
{
    // Calculates the vector of the texture coordinates edges, the distance between them.
    float tdistBAx, tdistBAy;
    tdistBAx = tBx - tAx;
    tdistBAy = tBy - tAy;

    float tdistCAx, tdistCAy;
    tdistCAx = tCx - tAx;
    tdistCAy = tCy - tAy;

    // Calculates the triangle's area.
    float area;
    area = tdistBAx * tdistCAy - tdistBAy * tdistCAx;

    //  Tangent
    if (area == 0.0f) {
        *tx = 0.0;
        *ty = 0.0;
        *tz = 0.0;
    } else {
        float delta = 1.0f / area;
        float distBAx, distBAy, distBAz;
        distBAx = pBx - pAx;
        distBAy = pBy - pAy;
        distBAz = pBz - pAz;

        float distCAx, distCAy, distCAz;
        distCAx = pCx - pAx;
        distCAy = pCy - pAy;
        distCAz = pCz - pAz;

        // Calculates the face tangent to the current triangle.
        *tx = delta * ((distBAx * tdistCAy) + (distCAx * -tdistBAy));
        *ty = delta * ((distBAy * tdistCAy) + (distCAy * -tdistBAy));
        *tz = delta * ((distBAz * tdistCAy) + (distCAz * -tdistBAy));
    }

    // Averages the new tagent vector with the oldest buffered.
    //tangentBuffer[i1] = vec3Add(tangent, tangentBuffer[i1]);
    //tangentBuffer[i2] = vec3Add(tangent, tangentBuffer[i2]);
    //tangentBuffer[i3] = vec3Add(tangent, tangentBuffer[i3]);
}
