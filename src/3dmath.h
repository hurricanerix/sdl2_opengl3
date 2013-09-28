#ifndef __3D_MATH_H__
#define __3D_MATH_H__

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

#endif //__3D_MATH_H__
