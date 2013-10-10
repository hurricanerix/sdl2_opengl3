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
#include "includes.h"


#define SHADER_DEFAULT_REF_COUNT (0)
#define SHADER_DEFAULT_VERTEX_LOC (0)
#define SHADER_DEFAULT_NORMAL_LOC (0)
#define SHADER_DEFAULT_TANGENT_LOC (0)
#define SHADER_DEFAULT_TEX_COORDS_LOC (0)
#define SHADER_DEFAULT_PROJ_MATRIX_LOC (0)
#define SHADER_DEFAULT_VIEW_MATRIX_LOC (0)
#define SHADER_DEFAULT_ROTATION_MATRIX_LOC (0)
#define SHADER_DEFAULT_VERT_FILENAME ('\0')
#define SHADER_DEFAULT_FRAG_FILENAME ('\0')
#define SHADER_DEFAULT_PROGRAM_ID (0)
#define SHADER_DEFAULT_VERT_PROGRAM_ID (0)
#define SHADER_DEFAULT_FRAG_PROGRAM_ID (0)


void _set_shader_defaults(Shader *s);


void init_shader(Shader *s)
{
    assert(s != NULL);

    init_status(&s->status);

    _set_shader_defaults(s);
}

void load_shader(Shader *s, char *vert_filename, char *frag_filename)
{
    assert(s != NULL);
    assert(vert_filename != NULL);
    assert(frag_filename != NULL);

    strncpy(s->vert_filename, (vert_filename), MAX_FILENAME_LEN);
    strncpy(s->frag_filename, (frag_filename), MAX_FILENAME_LEN);

    s->vert_program_id = glCreateShader(GL_VERTEX_SHADER);
    if (s->vert_program_id == 0) {
        set_error_msg(&s->status, "Could not create vert shader.");
        return;
    }

    s->frag_program_id = glCreateShader(GL_FRAGMENT_SHADER);
    if (s->frag_program_id == 0) {
        set_error_msg(&s->status, "Could not create frag shader.");
        return;
    }

    char *vs = NULL;
    int vs_size;
    vs = text_file_read(s->vert_filename, &vs_size);
    if (vs == NULL) {
        set_error_msg(&s->status, "Could not read vert shader '%s'.",
            s->vert_filename);
        return;
    }

    char *fs = NULL;
    int fs_size;
    fs = text_file_read(s->frag_filename, &fs_size);
    if (fs == NULL) {
        set_error_msg(&s->status, "Could not read frag shader '%s'.",
            s->frag_filename);
        return;
    }

    GLenum glerror;
    GLint result;
    const char * vv = vs;
    const char * ff = fs;

    glShaderSource(s->vert_program_id, 1, &vv, NULL);
    if ((glerror = glGetError()) != GL_NO_ERROR) {
        set_error_msg(&s->status, "Could set vert shader source. (%d)",
            glerror);
        return;
    }

    glShaderSource(s->frag_program_id, 1, &ff, NULL);
    if ((glerror = glGetError()) != GL_NO_ERROR) {
        set_error_msg(&s->status, "Could set frag shader source. (%d)",
            glerror);
        return;
    }

    glCompileShader(s->vert_program_id);
    glGetShaderiv(s->vert_program_id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        set_error_msg(&s->status, "Could not compile vert shader.");
        return;
    }

    glCompileShader(s->frag_program_id);
    glGetShaderiv(s->frag_program_id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        set_error_msg(&s->status, "Could not compile frag shader.");
        return;
    }

    free(vs); free(fs);

    print_shader_log(s->vert_program_id);
    print_shader_log(s->frag_program_id);

    s->program_id = glCreateProgram();
    if (s->program_id == 0) {
        set_error_msg(&s->status, "Could not create program.");
        return;
    }

    glAttachShader(s->program_id, s->vert_program_id);
    if ((glerror = glGetError()) != GL_NO_ERROR) {
        set_error_msg(&s->status, "Could not attach vert shader. (%d)",
            glerror);
        return;
    }

    glAttachShader(s->program_id, s->frag_program_id);
    if ((glerror = glGetError()) != GL_NO_ERROR) {
        set_error_msg(&s->status, "Could not attach frag shader. (%d)",
            glerror);
        return;
    }

    glBindFragDataLocation(s->program_id, 0, "FracColor");
    if ((glerror = glGetError()) != GL_NO_ERROR) {
        set_error_msg(&s->status, "Could not bind frag data location. (%d)",
            glerror);
        return;
    }

    glLinkProgram(s->program_id);
    glGetProgramiv(s->program_id, GL_LINK_STATUS, &result);
    if ((glerror = glGetError()) != GL_NO_ERROR || result == GL_FALSE) {
        set_error_msg(&s->status, "Could not bind frag data location. (%d,%d)",
            glerror, result);
        return;
    }

    print_program_log(s->program_id);

    s->vertex_loc = glGetAttribLocation(s->program_id, "MCvertex");
    s->normal_loc = glGetAttribLocation(s->program_id, "MCnormal");
    s->tangent_loc = glGetAttribLocation(s->program_id, "MCtangent");
    s->tex_coords_loc = glGetAttribLocation(s->program_id, "TexCoord0");

    s->proj_matrix_loc = glGetUniformLocation(s->program_id, "MVPMatrix");
    s->view_matrix_loc = glGetUniformLocation(s->program_id, "MVMatrix");
    s->rot_matrix_loc = glGetUniformLocation(s->program_id, "RotMatrix");
}

void bind_uniform(Shader *s, UniformConfig *config)
{
    assert(s != NULL);
    assert(config != NULL);

    GLuint loc = glGetUniformLocation(s->program_id, config->name);
    switch(config->type) {
    case UNIFORM_INT:
        glUniform1i(loc, config->i);
        break;
    case UNIFORM_FLOAT:
        glUniform1f(loc, config->x);
        break;
    case UNIFORM_VEC2:
        glUniform2f(loc, config->x, config->y);
        break;
    case UNIFORM_VEC3:
        glUniform3f(loc, config->x, config->y, config->z);
        break;
    case UNIFORM_VEC4:
        glUniform4f(loc, config->x, config->y, config->z, config->w);
        break;
    default:
        break;
    }
}

int print_gl_error(char *file, int line)
{
    assert(file != NULL);
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

    return retCode;
}

void print_shader_log(GLuint obj)
{

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
}

void print_program_log(GLuint obj)
{
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
}

void destroy_shader(Shader *s)
{
    if (s == NULL) {
        return;
    }

    if (s->vert_program_id != SHADER_DEFAULT_VERT_PROGRAM_ID) {
        glDeleteShader(s->vert_program_id);
    }

    if (s->frag_program_id != SHADER_DEFAULT_FRAG_PROGRAM_ID) {
        glDeleteShader(s->frag_program_id);
    }

    if (s->program_id != SHADER_DEFAULT_PROGRAM_ID) {
        glDeleteProgram(s->program_id);
    }

    free(s->vert_filename);
    free(s->frag_filename);

    _set_shader_defaults(s);
}

void _set_shader_defaults(Shader *s)
{
    assert(s != NULL);

    s->ref_count = SHADER_DEFAULT_REF_COUNT;
    s->vertex_loc = SHADER_DEFAULT_VERTEX_LOC;
    s->normal_loc = SHADER_DEFAULT_NORMAL_LOC;
    s->tangent_loc = SHADER_DEFAULT_TANGENT_LOC;
    s->tex_coords_loc = SHADER_DEFAULT_TEX_COORDS_LOC;
    s->proj_matrix_loc = SHADER_DEFAULT_PROJ_MATRIX_LOC;
    s->view_matrix_loc = SHADER_DEFAULT_VIEW_MATRIX_LOC;
    s->rot_matrix_loc = SHADER_DEFAULT_ROTATION_MATRIX_LOC;
    s->vert_filename[0] = SHADER_DEFAULT_VERT_FILENAME;
    s->frag_filename[0] = SHADER_DEFAULT_FRAG_FILENAME;
    s->program_id = SHADER_DEFAULT_PROGRAM_ID;
    s->vert_program_id = SHADER_DEFAULT_VERT_PROGRAM_ID;
    s->frag_program_id = SHADER_DEFAULT_FRAG_PROGRAM_ID;
}

void _print_shader(FILE *fp, Shader *s)
{
    assert(fp != NULL);
    assert(s != NULL);

    fprintf(fp, "ref_count:\t%d\n", s->ref_count);
    fprintf(fp, "vertex_loc:\t%d\n", s->vertex_loc);
    fprintf(fp, "normal_loc:\t%d\n", s->normal_loc);
    fprintf(fp, "tangent_loc:\t%d\n", s->tangent_loc);
    /*GLuint tex_coords_loc;
    GLuint proj_matrix_loc;
    GLuint view_matrix_loc;
    GLuint rot_matrix_loc;
    */
    fprintf(fp, "vert_filename:\t%s\n", s->vert_filename);
    fprintf(fp, "frag_filename:\t%s\n", s->frag_filename);
    fprintf(fp, "program_id:\t%d\n", s->program_id);
    fprintf(fp, "vert_program_id:\t%d\n", s->vert_program_id);
    fprintf(fp, "frag_program_id:\t%d\n", s->frag_program_id);
    // Status status;
}
