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
#include "includes.h"


void print_vec2(char *label, vec2 v)
{
    if (label != NULL) {
        printf("vec2 %s\n", label);
    }

    printf("%f,\t%f\n", v.x, v.y);
}

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

void print_mat3(char *label, mat3 m)
{
    if (label != NULL) {
        printf("mat3 %s\n", label);
    }

    printf("[\n");
    print_vec3(NULL, m.row1);
    print_vec3(NULL, m.row2);
    print_vec3(NULL, m.row3);
    printf("]\n");
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

mat3 get_rotation_matrix(vec3 dir)
{
    float c1 = cos(dir.x);
    float c2 = cos(dir.y);
    float c3 = cos(dir.z);
    float s1 = sin(dir.x);
    float s2 = sin(dir.y);
    float s3 = sin(dir.z);

    mat3 m;
    m.row1.x = c2 * c3;
    m.row1.y = -c2 * s3;
    m.row1.z = s2;
    m.row2.x = (c1 * s3) + (c3 * s1 * s2);
    m.row2.y = (c1 * c3) - (s1 * s2 * s3);
    m.row2.z = -c2 * s1;
    m.row3.x = (s1 * s3) - (c1 * c3 * s2);
    m.row3.y = (c3 * s1) + (c1 * s2 * s3);
    m.row3.z = c1 * c2;

    return m;
}

// ----------------------------------------------------
// VECTOR STUFF
//

// res = a cross b;
vec3 cross_product(vec3 a, vec3 b)
{
    vec3 res;

    res.x = a.y * b.z  -  b.y * a.z;
    res.y = a.z * b.x  -  b.z * a.x;
    res.z = a.x * b.y  -  b.x * a.y;

    return res;
}

// Normalize a vec3
vec3 normalize(vec3 a)
{
    float mag = sqrt(a.x * a.x  +  a.y * a.y  +  a.z * a.z);

    if (mag == 0) {
        a.x = 0.0;
        a.y = 0.0;
        a.z = 0.0;
        return a;
    }

    a.x /= mag;
    a.y /= mag;
    a.z /= mag;

    return a;
}

// ----------------------------------------------------
// MATRIX STUFF
//

// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)

//
// a = a * b;
//

// Defines a transformation matrix mat with a translation

// ----------------------------------------------------
// Projection Matrix
//

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

    return res;
}

mat4 get_translation_mat4(vec3 v)
{
    mat4 m = get_identity_mat4();
    m.row4.x = v.x;
    m.row4.y = v.y;
    m.row4.z = v.z;

    return m;
}


// ----------------------------------------------------
// View Matrix
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector (remmeber gluLookAt?)
//


mat4 get_view_matrix(vec3 pos, vec3 lookAt)
{
    vec3 dir = {{.x=lookAt.x - pos.x}, {.y=lookAt.y - pos.y}, {.z=lookAt.z-pos.z}};
    vec3 right = {{.x=0.0}, {.y=0.0}, {.z=0.0}};
    vec3 up = {{.x=0.0}, {.y=1.0}, {.z=0.0}};

    dir = normalize(dir);

    right = cross_product(dir, up);
    right = normalize(right);

    up = cross_product(right, dir);
    up = normalize(up);

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

    pos.x *= -1;
    pos.y *= -1;
    pos.z *= -1;
    mat4 aux = get_translation_mat4(pos);
    view_matrix = mult_mat4(view_matrix, aux);

    return view_matrix;
}

// Get Surface Local Tangent
vec3 get_surface_local_tangent(
        vec3 p1, vec2 t1, vec3 p2, vec2 t2, vec3 p3, vec2 t3)
{
    // Calculates the vector of the texture coordinates edges, the distance between them.
    vec2 tdistBA;
    tdistBA.x = t2.x - t1.x;
    tdistBA.y = t2.y - t1.y;

    vec2 tdistCA;
    tdistCA.x = t3.x - t1.x;
    tdistCA.y = t3.y - t1.y;

    // Calculates the triangle's area.
    float area;
    area = tdistBA.x * tdistCA.y - tdistBA.y * tdistCA.x;

    //  Tangent
    vec3 t;
    if (area == 0.0f) {
        t.x = 0.0;
        t.y = 0.0;
        t.z = 0.0;
    } else {
        float delta = 1.0f / area;
        vec3 distBA;
        distBA.x = p2.x - p1.x;
        distBA.y = p2.y - p1.y;
        distBA.z = p2.z - p1.z;

        vec3 distCA;
        distCA.x = p3.x - p1.x;
        distCA.y = p3.y - p1.y;
        distCA.z = p3.z - p1.z;

        // Calculates the face tangent to the current triangle.
        t.x = delta * ((distBA.x * tdistCA.y) + (distCA.x * -tdistBA.y));
        t.y = delta * ((distBA.y * tdistCA.y) + (distCA.y * -tdistBA.y));
        t.z = delta * ((distBA.z * tdistCA.y) + (distCA.z * -tdistBA.y));
    }

    t.x = 1.0;
    t.y = 1.0;
    t.z = 1.0;

    // Averages the new tagent vector with the oldest buffered.
    //tangentBuffer[i1] = vec3Add(tangent, tangentBuffer[i1]);
    //tangentBuffer[i2] = vec3Add(tangent, tangentBuffer[i2]);
    //tangentBuffer[i3] = vec3Add(tangent, tangentBuffer[i3]);
    return t;
}
