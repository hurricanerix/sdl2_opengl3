#ifndef __SHADER_H__
#define __SHADER_H__

extern GLuint vertexLoc, colorLoc;
extern GLuint projMatrixLoc, viewMatrixLoc;
extern GLuint rotXLoc, rotYLoc, rotZLoc;

#define printOpenGLError() printOglError(__FILE__, __LINE__)
GLuint setupShaders(char *vertexFileName, char *fragmentFileName);

#endif// __SHADER_H__
