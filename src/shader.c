#define GL3_PROTOTYPES 1
#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl3.h>

#include "shader.h"

const int MAX_SHADER_FILE_SIZE = 102400;

GLuint vert_shader_id;
GLuint frag_shader_id;

GLuint get_vert_shader_id() {
    return vert_shader_id;
}

GLuint get_frag_shader_id() {
    return frag_shader_id;
}

void print_program_log(GLint program_id) {
    int log_length;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length <= 0) {
        printf("---{log-start}---\n");
        printf("NULL\n");
        printf("---{log-stop}---\n");
        return;
     }

    char *log;
    int len_read;
    log = malloc(log_length);
    if (log == NULL) {
        fprintf(stderr, "ERROR(SHADER): Could not malloc for program log.\n");
        return;
    }

    glGetProgramInfoLog(program_id, log_length, &len_read, log);
    if (len_read < 0) {
       fprintf(stderr, "ERROR(SHADER): Could not read program log.\n");
       free (log);
       return;
    }

    if ((log_length - 1) != len_read) {
        fprintf(stderr, "ERROR(SHADER): Could not read entire program log.\n");
        log[len_read] = '\0';
    }

    printf("---{log-start}---\n");
    printf("%s", log);
    printf("---{log-stop}---\n");

    free(log);
}

void print_shader_log(GLint shader_id, char *name) {
    int log_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length <= 0 ){
        printf("---{log-start}---\n");
        printf("NULL\n");
        printf("---{log-stop}---\n");
        return;
    }

    char *log;
    int len_read;
    log = malloc(log_length);
    if (log == NULL) {
        fprintf(stderr, "ERROR(SHADER): Could not malloc for shader log.\n");
        return;
    }

    glGetShaderInfoLog(shader_id, log_length, &len_read, log);
    if (len_read < 0) {
       fprintf(stderr, "ERROR(SHADER): Could not read shader log.\n");
       free (log);
       return;
    }

    if ((log_length -1) != len_read) {
        fprintf(stderr, "ERROR(SHADER): Could not read entire shader log.\n");
        log[len_read] = '\0';
    }

    printf("---{log-start}---\n");
    printf("%s", log);
    printf("---{log-stop}---\n");

    free(log);
}

GLuint compile_shader(char *filename, GLuint type) {
    printf("-- SHADER -- Reading shader: %s\n", filename);
    char buf[MAX_SHADER_FILE_SIZE];
    FILE *file;
    size_t nread;

    file = fopen(filename, "r");
    if (file) {
        nread = fread(buf, 1, sizeof(buf), file);
        fclose(file);
    }
    buf[nread] = '\0';

    printf("-- SHADER -- Reading shader complete\n");
    printf("---{src-start}---\n");
    printf("%s", buf);
    printf("---{src-stop}---\n");

    GLuint shader_id = glCreateShader(type);


    printf("-- SHADER -- Compiling shader\n");
    char const *shader_src = (char const *) &buf;
    glShaderSource(shader_id, 1, &shader_src, NULL);
    glCompileShader(shader_id);

    GLint result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        printf("ERROR(SHADER): Compiling shader\n");
        print_shader_log(shader_id, filename);
        glDeleteShader(shader_id);
        return -1;
    }
    printf("-- SHADER -- Compiling shader complete\n");

    return shader_id;
}

GLuint install_shader() {
    GLint result = GL_FALSE;

    GLuint program_id;
    if ((program_id = glCreateProgram()) == 0) {
        fprintf(stderr, "ERROR(SHADER): Could not create GLSL program.\n");
        fprintf(stderr, "ERROR(SHADER): Could not install shader.\n");
        return -1;
    }
    printf("-- SHADER -- Created program: %d\n", program_id);

    //GLuint vert_shader_id;
    char vert_filename[] = "resources/shaders/basic.vert";
    if ((vert_shader_id =
            compile_shader(vert_filename, GL_VERTEX_SHADER)) == -1) {
        fprintf(stderr, "ERROR(SHADER): Could not install shader.\n");
        glDeleteShader(vert_shader_id);
        return -1;
    }
    printf("-- SHADER -- Created vert shader: %d\n", vert_shader_id);

    glAttachShader(program_id, vert_shader_id);
    /*result = glGetError();
    if (result == GL_INVALID_VALUE || result == GL_INVALID_OPERATION) { 
        fprintf(stderr, "ERROR(SHADER): Could not attach vertex shader to program.\n");
        fprintf(stderr, "ERROR(SHADER): Could not install shader.\n");
        print_program_log(program_id);
        glDeleteShader(vert_shader_id);
        return -1;
    }*/
    printf("-- SHADER -- Attached vert shader\n");

    //GLuint frag_shader_id;
    char frag_filename[] = "resources/shaders/basic.frag";
    if ((frag_shader_id =
            compile_shader(frag_filename, GL_FRAGMENT_SHADER)) == -1) {
        fprintf(stderr, "ERROR(SHADER): Could not install shader.\n");
        glDeleteShader(vert_shader_id);
        glDeleteShader(frag_shader_id);
        return -1;
    }
    printf("-- SHADER -- Created frag shader: %d\n", frag_shader_id);

    glAttachShader(program_id, frag_shader_id);
    /*result = glGetError();
    if (result == GL_INVALID_VALUE || result == GL_INVALID_OPERATION) { 
        fprintf(stderr, "ERROR(SHADER): Could not attach fragment shader to program.\n");
        fprintf(stderr, "ERROR(SHADER): Could not install shader.\n");
        print_program_log(program_id);
        glDeleteShader(vert_shader_id);
        glDeleteShader(frag_shader_id);
        return -1;
    }*/
    printf("-- SHADER -- Attached frag shader\n");

    GLint link_status;
    glLinkProgram(program_id);
    result = glGetError();
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE || result == GL_INVALID_VALUE ||
            result == GL_INVALID_OPERATION) {
        fprintf(stderr, "ERROR(SHADER): Could not link shaders to program.\n");
        fprintf(stderr, "ERROR(SHADER): Could not install shader.\n");
        print_program_log(program_id);
        glDeleteShader(vert_shader_id);
        glDeleteShader(frag_shader_id);
        return -1;
    }
    printf("-- SHADER -- Linked program to shaders\n");

    glDeleteShader(vert_shader_id);
    result = glGetError();
    if (result == GL_INVALID_VALUE || result == GL_INVALID_OPERATION) { 
        fprintf(stderr, "ERROR(SHADER): Could not delete vertex shader.\n");
    }
    printf("-- SHADER -- Deleted vert shader\n");

    glDeleteShader(frag_shader_id);
    result = glGetError();
    if (result == GL_INVALID_VALUE || result == GL_INVALID_OPERATION) { 
        fprintf(stderr, "ERROR(SHADER): Could not delete fragment shader.\n");
    }
    printf("-- SHADER -- Deleted frag shader\n");

    return program_id;
}
