#define GL3_PROTOTYPES 1
#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

#include "shader.h"

const int MAX_SHADER_FILE_SIZE = 102400;

void print_program_log(GLint program_id) {
    int log_length;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length <= 0) {
        printf("Program Log: NULL\n");
        return;
     }

    char *log;
    int len_read;
    log = malloc(log_length);
    if (log == NULL) {
        fprintf(stderr, "ERROR: Could not malloc for program log.\n");
        return;
    }

    glGetProgramInfoLog(program_id, log_length, &len_read, log);
    if (len_read < 0) {
       fprintf(stderr, "ERROR: Could not read program log.\n");
       free (log);
       return;
    }

    if (log_length != len_read) {
        fprintf(stderr, "ERROR: Could not read entire program log.\n");
        log[len_read] = '\0';
    }

    printf("%s\n", log);

    free(log);
}

void print_shader_log(GLint shader_id) {
    int log_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length <= 0 ){
        printf("Shader Log: NULL\n");
        return;
    }

    char *log;
    int len_read;
    log = malloc(log_length);
    if (log == NULL) {
        fprintf(stderr, "ERROR: Could not malloc for shader log.\n");
        return;
    }

    glGetShaderInfoLog(shader_id, log_length, &len_read, log);
    if (len_read < 0) {
       fprintf(stderr, "ERROR: Could not read shader log.\n");
       free (log);
       return;
    }

    if (log_length != len_read) {
        fprintf(stderr, "ERROR: Could not read entire shader log.\n");
        log[len_read] = '\0';
    }

    printf("%s\n", log);

    free(log);
}

GLuint compile_shader(char *filename, GLuint type) {
    printf("Reading shader: %s\n", filename);
    char buf[MAX_SHADER_FILE_SIZE];
    printf("TEST: %d\n", sizeof(buf));
    FILE *file;
    size_t nread;

    file = fopen(filename, "r");
    if (file) {
        nread = fread(buf, 1, sizeof(buf), file);
        fclose(file);
    }
    buf[nread] = '\0';

    GLuint shader_id = glCreateShader(type);

    printf("Compiling shader: \n");
    char const *shader_src = &buf;
    glShaderSource(shader_id, 1, &shader_src, NULL);
    glCompileShader(shader_id);

    GLint result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    print_shader_log(shader_id);

    return shader_id;
}

GLuint install_shader() {
    GLint result = GL_FALSE;

    GLuint program_id;
    if ((program_id = glCreateProgram()) == 0) {
        fprintf(stderr, "ERROR: Could not create GLSL program.\n");
        fprintf(stderr, "ERROR: Could not install shader.\n");
        return -1;
    }

    GLuint vert_shader_id;
    char vert_filename[] = "resources/shaders/basic_120.vert";
    if ((vert_shader_id =
            compile_shader(vert_filename, GL_VERTEX_SHADER)) == -1) {
        fprintf(stderr, "ERROR: Could not install shader.\n");
        glDeleteShader(vert_shader_id);
        return -1;
    }

    glAttachShader(program_id, vert_shader_id);
    result = glGetError();
    if (result == GL_INVALID_VALUE || result == GL_INVALID_OPERATION) { 
        fprintf(stderr, "ERROR: Could not attach vertex shader to program.\n");
        fprintf(stderr, "ERROR: Could not install shader.\n");
        print_program_log(program_id);
        glDeleteShader(vert_shader_id);
        return -1;
    }

    GLuint frag_shader_id;
    char frag_filename[] = "resources/shaders/basic_120.frag";
    if ((frag_shader_id =
            compile_shader(frag_filename, GL_FRAGMENT_SHADER)) == -1) {
        fprintf(stderr, "ERROR: Could not install shader.\n");
        glDeleteShader(vert_shader_id);
        glDeleteShader(frag_shader_id);
        return -1;
    }

    glAttachShader(program_id, frag_shader_id);
    result = glGetError();
    if (result == GL_INVALID_VALUE || result == GL_INVALID_OPERATION) { 
        fprintf(stderr, "ERROR: Could not attach fragment shader to program.\n");
        fprintf(stderr, "ERROR: Could not install shader.\n");
        print_program_log(program_id);
        glDeleteShader(vert_shader_id);
        glDeleteShader(frag_shader_id);
        return -1;
    }

    GLint link_status;
    glLinkProgram(program_id);
    result = glGetError();
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE || result == GL_INVALID_VALUE ||
            result == GL_INVALID_OPERATION) {
        fprintf(stderr, "ERROR: Could not link shaders to program.\n");
        fprintf(stderr, "ERROR: Could not install shader.\n");
        print_program_log(program_id);
        glDeleteShader(vert_shader_id);
        glDeleteShader(frag_shader_id);
        return -1;
    }

    glDeleteShader(vert_shader_id);
    result = glGetError();
    if (result == GL_INVALID_VALUE || result == GL_INVALID_OPERATION) { 
        fprintf(stderr, "ERROR: Could not delete vertex shader.\n");
    }

    glDeleteShader(frag_shader_id);
    result = glGetError();
    if (result == GL_INVALID_VALUE || result == GL_INVALID_OPERATION) { 
        fprintf(stderr, "ERROR: Could not delete fragment shader.\n");
    }

    return program_id;
}
