#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl3.h>

#include "logger.h"
#include "shader.h"
#include "text.h"


GLuint vertexLoc, colorLoc;
GLuint projMatrixLoc, viewMatrixLoc;
GLuint rotMatrixLoc;


int printOglError(char *file, int line)
{
    assert(file != NULL);
    log_debug("printOglError {");
    log_debug("  -in- file - %s", file);
    log_debug("  -in- line - %d", line);
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        //printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        printf("glError in file %s @ line %d: \n", file, line);
        retCode = 1;
        glErr = glGetError();
    }

    log_debug("printOglError }");
    log_debug("  -out- retCode - %d", retCode);
    return retCode;
}

void printShaderInfoLog(GLuint obj)
{
    log_debug("printShaderInfoLog {");
    log_debug("  -in- obj - %d", obj);

    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0) {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }

    log_debug("printShaderInfoLog }");
}

void printProgramInfoLog(GLuint obj)
{
    log_debug("printProgramInfoLog {");
    log_debug("  -in- obj - %d", obj);
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }

    log_debug("printProgramInfoLog }");
}

GLuint setupShaders(char *vertexFileName, char *fragmentFileName)
{
    assert(vertexFileName != NULL);
    assert(fragmentFileName != NULL);
    log_debug("setupShaders {");
    log_debug("  -in- vertexFileName - %s", vertexFileName);
    log_debug("  -in- fragmentFileName - %s", fragmentFileName);

    char *vs = NULL,*fs = NULL;

    GLuint p,v,f;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead(vertexFileName);
    fs = textFileRead(fragmentFileName);

    const char * vv = vs;
    const char * ff = fs;

    glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);

    free(vs);free(fs);

    glCompileShader(v);
    glCompileShader(f);

    printShaderInfoLog(v);
    printShaderInfoLog(f);

    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);

    glBindFragDataLocation(p, 0, "outputF");
    glLinkProgram(p);
    printProgramInfoLog(p);

    vertexLoc = glGetAttribLocation(p,"position");
    colorLoc = glGetAttribLocation(p, "color"); 

    projMatrixLoc = glGetUniformLocation(p, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
    rotMatrixLoc = glGetUniformLocation(p, "rotMatrix");

    log_debug("setupShaders }");
    return(p);
}
