#ifndef __3D_MATH_H__
#define __3D_MATH_H__

void get_x_rot_matrix(float *m, float angle);
void get_y_rot_matrix(float *m, float angle);
void get_z_rot_matrix(float *m, float angle);

void crossProduct( float *a, float *b, float *res);
void normalize(float *a);
void setIdentityMatrix( float *mat, int size);
void multMatrix(float *a, float *b);
void setTranslationMatrix(float *mat, float x, float y, float z);
void buildProjectionMatrix(float *projMatrix, float fov, float ratio, float nearP, float farP);
void setCamera(float *viewMatrix, float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ);

#endif //__3D_MATH_H__
