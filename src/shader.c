#define GL3_PROTOTYPES 1
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

#include "shader.h"


char vert_shader_src[] =
    "#version 120\n"
    "void main(void) {"
    "    gl_Position = ftransform();"
    "}";

char frag_shader_src[] =
    "#version 120\n"
    "void main(void) {"
    "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
    "}";

GLuint load_shaders() {
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling vert shader : \n");
    char const * VertexSourcePointer = &vert_shader_src;
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error compiling vert shader\n.");
    char *log;
    int bufsz;
    int len; 

    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &bufsz);
    if (bufsz <= 0) {
        //fprintf(stderr, "glGetProgramiv: GL_INFO_LOG_LENGTH == %d\n", bufsz);
        return;
    }
    
    log = malloc(bufsz);
    if (log == NULL) {
        fprintf(stderr, "glGetShaderiv: malloc returned NULL pointer\n");
        // TODO: print error?
        return;
    }

    glGetShaderInfoLog(VertexShaderID, bufsz, &len, log);
    if (len <= 0) {
       //fprintf(stderr, "glGetProgramiv: len == %d\n", len);
       // TODO: print message?
       free (log);
       return; 
    }

    if (bufsz != len) {
        fprintf(stderr, "glGetProgramiv: bufsz != len\n");
        log[len] = '\0';
    }

    printf("%s\n", log);

    free(log);
        //std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        //glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        //printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling frag shader : \n");
    char const * FragmentSourcePointer = &frag_shader_src;
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error compiling frag shader\n.");
    char *log;
    int bufsz;
    int len; 

    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &bufsz);
    if (bufsz <= 0) {
        //fprintf(stderr, "glGetProgramiv: GL_INFO_LOG_LENGTH == %d\n", bufsz);
        return;
    }
    
    log = malloc(bufsz);
    if (log == NULL) {
        fprintf(stderr, "glGetShaderiv: malloc returned NULL pointer\n");
        // TODO: print error?
        return;
    }

    glGetShaderInfoLog(FragmentShaderID, bufsz, &len, log);
    if (len <= 0) {
       //fprintf(stderr, "glGetProgramiv: len == %d\n", len);
       // TODO: print message?
       free (log);
       return; 
    }

    if (bufsz != len) {
        fprintf(stderr, "glGetProgramiv: bufsz != len\n");
        log[len] = '\0';
    }

    printf("%s\n", log);

    free(log);
        //std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        //glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        //printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error compiling program\n.");
    char *log;
    int bufsz;
    int len; 

    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &bufsz);
    if (bufsz <= 0) {
        return;
    }
    
    log = malloc(bufsz);
    if (log == NULL) {
        fprintf(stderr, "glGetProgramiv: malloc returned NULL pointer\n");
        // TODO: print error?
        return;
    }

    glGetProgramInfoLog(ProgramID, bufsz, &len, log);
    if (len < 0) {
       free (log);
       return; 
    }

    if (bufsz != len) {
        //fprintf(stderr, "glGetProgramiv: bufsz != len\n");
        log[len] = '\0';
    }

    printf("%s\n", log);

    free(log);
        //std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        //glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        //printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}
