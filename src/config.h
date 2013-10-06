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
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "main.h"
#include "status.h"

#define MAX_LINE_LEN (1024)
#define MAX_UNIFORM_COUNT (10)
#define MAX_TEXTURE_COUNT (4)

typedef struct AppConfig {
    char object_filename[MAX_FILENAME_LEN];
} AppConfig;

typedef struct ObjectConfig {
    char vert_shader_filename[MAX_FILENAME_LEN];
    char frag_shader_filename[MAX_FILENAME_LEN];
} ObjectConfig;

enum UniformDataTypes {
    UNIFORM_UNKNOWN, UNIFORM_INT, UNIFORM_FLOAT,
    UNIFORM_VEC2, UNIFORM_VEC3, UNIFORM_VEC4};

typedef struct UniformConfig {
    enum UniformDataTypes type;
    char name[MAX_LINE_LEN];
    int i;
    float x;
    float y;
    float z;
    float w;
} UniformConfig;

typedef struct TextureConfig {
    char name[MAX_LINE_LEN];
    char filename[MAX_LINE_LEN];
} TextureConfig;

typedef struct Config {
    Status status;
    AppConfig app;
    ObjectConfig object;
    int vert_uniform_count;
    UniformConfig vert_uniforms[MAX_UNIFORM_COUNT];
    int frag_uniform_count;
    UniformConfig frag_uniforms[MAX_UNIFORM_COUNT];
    int texture_count;
    TextureConfig textures[MAX_TEXTURE_COUNT];
} Config;

void init_config(Config *c);
void load_config(Config *c, char *filename);
void destroy_config(Config *config);

void print_config(Config *config);
void print_app_config(AppConfig *config);
void print_object_config(ObjectConfig *config);
void print_texture_config(TextureConfig *config);
void print_uniform_config(UniformConfig *config);

#endif//__CONFIG_H__
