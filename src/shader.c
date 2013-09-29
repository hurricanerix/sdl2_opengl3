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
#include "shader.h"

#include "logger.h"
#include "text.h"


GLuint vertexLoc, normalLoc;
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

    int vs_size;
    vs = text_file_read(vertexFileName, &vs_size);
    int fs_size;
    fs = text_file_read(fragmentFileName, &fs_size);

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

    glBindFragDataLocation(p, 0, "FracColor");
    glLinkProgram(p);
    printProgramInfoLog(p);

    vertexLoc = glGetAttribLocation(p,"MCvertex");
    normalLoc = glGetAttribLocation(p, "MCnormal"); 

    projMatrixLoc = glGetUniformLocation(p, "MVPMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "MVMatrix");
    rotMatrixLoc = glGetUniformLocation(p, "RotMatrix");

    log_debug("setupShaders }");
    return(p);
}
