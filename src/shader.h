#ifndef __SHADER_H__
#define __SHADER_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl3.h>
#endif

extern GLuint vertexLoc, normalLoc;
extern GLuint projMatrixLoc, viewMatrixLoc;
extern GLuint rotMatrixLoc;

#define printOpenGLError() printOglError(__FILE__, __LINE__)
GLuint setupShaders(char *vertexFileName, char *fragmentFileName);

#endif// __SHADER_H__
