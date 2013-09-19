#include <assert.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>

#include "3dmath.h"

float rotXMatrix[9] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 9.9f, 9.9f,
    0.0f, 9.9f, 9.9f};

float rotYMatrix[9] = {
    9.9f, 0.0f, 9.9f,
    0.0f, 1.0f, 0.0f,
    9.9f, 0.0f, 9.9f};

float rotZMatrix[9] = {
    9.9f, 9.9f, 0.0f,
    9.9f, 9.9f, 0.0f,
    0.0f, 0.0f, 1.0f};

void get_x_rot_matrix(float *m, float angle) {
    assert(m != NULL);

    // x
    rotXMatrix[4] = cos(angle);
    rotXMatrix[5] = -sin(angle);
    rotXMatrix[7] = sin(angle);
    rotXMatrix[8] = cos(angle);
}

void get_y_rot_matrix(float *m, float angle) {
    assert(m != NULL);
    // y
    rotYMatrix[0] = cos(angle);
    rotYMatrix[2] = sin(angle);
    rotYMatrix[6] = -sin(angle);
    rotYMatrix[8] = cos(angle);
}

void get_z_rot_matrix(float *m, float angle) {
    assert(m != NULL);
    // z
    rotZMatrix[0] = cos(angle);
    rotZMatrix[1] = -sin(angle);
    rotZMatrix[3] = sin(angle);
    rotZMatrix[4] = cos(angle);
}

// ----------------------------------------------------
// VECTOR STUFF
//

// res = a cross b;
void crossProduct( float *a, float *b, float *res) {

    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}

// Normalize a vec3
void normalize(float *a) {

    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);

    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}

// ----------------------------------------------------
// MATRIX STUFF
//

// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void setIdentityMatrix( float *mat, int size) {

    // fill matrix with 0s
    for (int i = 0; i < size * size; ++i)
            mat[i] = 0.0f;

    // fill diagonal with 1s
    for (int i = 0; i < size; ++i)
        mat[i + i * size] = 1.0f;
}

//
// a = a * b;
//
void multMatrix(float *a, float *b) {

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
void setTranslationMatrix(float *mat, float x, float y, float z) {

    setIdentityMatrix(mat,4);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
}

// ----------------------------------------------------
// Projection Matrix
//

void buildProjectionMatrix(
        float *projMatrix, float fov, float ratio, float nearP, float farP) {
    assert(projMatrix != NULL);

    float f = 1.0f / tan (fov * (M_PI / 360.0));

    setIdentityMatrix(projMatrix,4);

    projMatrix[0] = f / ratio;
    projMatrix[1 * 4 + 1] = f;
    projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    projMatrix[2 * 4 + 3] = -1.0f;
    projMatrix[3 * 4 + 3] = 0.0f;
}

// ----------------------------------------------------
// View Matrix
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector (remmeber gluLookAt?)
//

void setCamera(float *viewMatrix, float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ) {
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

// ----------------------------------------------------
