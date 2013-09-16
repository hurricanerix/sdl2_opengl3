#ifndef __3DMATH_H__
#define __3DMATH_H__

typedef struct vec3f {
    float x;
    float y;
    float z;
} vec3f;

typedef struct mat4f {
    char[16] data;
} mat4f;

int calculate_model_view_matrix(
    mat4f *mvm, vec3f eye, vec3f center, vec3f up);

int mat4f_set_identity_matrix(mat4f *x);

int mat4f_mult(mat4f *result, mat4f x, mat4f y);

int mat4f_add_vec3f(mat4f *result, mat4f x, vec3f y);

int vec3f_subtract(vec3f *result, vec3f x, vec3f y);

int vec3f_normalize(vec3f *x);

int vec3f_cross(vec3f *result, vec3f x, vec3f y);

int vec3f_negate(vec3f *result);

#endif // __3DMATH_H__
