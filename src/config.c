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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "logger.h"
#include "main.h"
#include "text.h"


#define CONFIG_DEFAULT_VERT_UNIFORM_COUNT (0)
#define CONFIG_DEFAULT_FRAG_UNIFORM_COUNT (0)
#define CONFIG_DEFAULT_TEXTURE_COUNT (0)

#define SECTION_START_TOKEN ('[')
#define SECTION_END_TOKEN (']')
#define COMMENT_TOKEN (';')
#define KEY_VALUE_DELIMITER ('=')
#define SPACE_TOKEN (' ')
#define EMPTY_LINE_TOKEN ('\0')

enum ConfigSections {
    UNKNOWN_SECTION, APP_SECTION, OBJECT_SECTION,
    TEXTURES_SECTION, VERT_SECTION, FRAG_SECTION};

void _set_config_defaults(Config *c);
void _init_app_config(AppConfig *c);
void _destroy_app_config(AppConfig *c);
void _set_app_config_defaults(AppConfig *c);
void _init_object_config(ObjectConfig *c);
void _destroy_object_config(ObjectConfig *c);
void _set_object_config_defaults(ObjectConfig *c);
void _init_texture_config(TextureConfig *c);
void _destroy_texture_config(TextureConfig *c);
void _set_texture_config_defaults(TextureConfig *c);
void _init_uniform_config(UniformConfig *c);
void _destroy_uniform_config(UniformConfig *c);
void _set_uniform_config_defaults(UniformConfig *c);


void init_config(Config *c)
{
    assert(c != NULL);

    init_status(&c->status);
    _init_app_config(&c->app);
    _init_object_config(&c->object);

    for (int i = 0; i < MAX_TEXTURE_COUNT; i++) {
        _init_texture_config(&c->textures[i]);
    }
    for (int i = 0; i < MAX_UNIFORM_COUNT; i++) {
        _init_uniform_config(&c->vert_uniforms[i]);
    }

    for (int i = 0; i < MAX_UNIFORM_COUNT; i++) {
        _init_uniform_config(&c->frag_uniforms[i]);
    }

    _set_config_defaults(c);
}

void load_config(Config *c, char *filename)
{
    assert(c != NULL);
    assert(filename != NULL);

    int ini_size;
    char *ini_data = text_file_read(filename, &ini_size);
    // TODO: test data was read in.

    char buffer[MAX_LINE_LEN];
    enum ConfigSections current_section = UNKNOWN_SECTION;
    char *key;
    char *value;
    while (get_next_line(ini_data, ini_size, buffer, MAX_LINE_LEN) == TRUE) {
        switch(buffer[0]) {
        case COMMENT_TOKEN:
            break; // Ignore this.
        case EMPTY_LINE_TOKEN:
            break; // Ignore this.
        case SPACE_TOKEN:
            break; // Ignore this.
        case SECTION_START_TOKEN:
            current_section = UNKNOWN_SECTION;
            value = strtok(&buffer[1], "]");
            assert(value != NULL);

            if (strncmp(value, "app", sizeof("app")) == 0) {
                current_section = APP_SECTION;
            }
            if (strncmp(value, "object", sizeof("object")) == 0) {
                current_section = OBJECT_SECTION;
            }
            if (strncmp(value, "textures", sizeof("textures")) == 0) {
                current_section = TEXTURES_SECTION;
            }
            if (strncmp(value, "vert_shader", sizeof("vert_shader")) == 0) {
                current_section = VERT_SECTION;
            }
            if (strncmp(value, "frag_shader", sizeof("frag_shader")) == 0) {
                current_section = FRAG_SECTION;
            }

            break;
        default:
            if (current_section != APP_SECTION &&
                    current_section != OBJECT_SECTION &&
                    current_section != TEXTURES_SECTION &&
                    current_section != VERT_SECTION &&
                    current_section != FRAG_SECTION) {
                continue;
            }

            key = strtok(&buffer[0], "=");
            assert(key != NULL);

            value = strtok(NULL, "\0");
            assert(value != NULL);

            if (current_section == APP_SECTION) {
                if (strncmp(key, "object_file", sizeof("object_file")) == 0) {
                    strncpy(c->app.object_filename, value, MAX_FILENAME_LEN);
                }
            }

            if (current_section == OBJECT_SECTION) {
                if (strncmp(key, "vert_shader_file", sizeof("vert_shader_file")) == 0) {
                    strncpy(c->object.vert_shader_filename, value, MAX_FILENAME_LEN);
                }
                else if (strncmp(key, "frag_shader_file", sizeof("frag_shader_file")) == 0) {
                    strncpy(c->object.frag_shader_filename, value, MAX_FILENAME_LEN);
                }
            }

            if (current_section == TEXTURES_SECTION) {
                assert(c->texture_count + 1 != MAX_TEXTURE_COUNT);
                TextureConfig *t;
                t = &(c->textures[c->texture_count]);
                strncpy(t->name, key, MAX_LINE_LEN);
                strncpy(t->filename, value, MAX_LINE_LEN);
                c->texture_count++;
            }

            if (current_section == VERT_SECTION || current_section == FRAG_SECTION) {
                char *type;
                enum UniformDataTypes etype;
                char *name;
                type = strtok(key, " ");
                name = strtok(NULL, "=");
                int i = 0;
                char *p;
                float x = 0.0;
                float y = 0.0;
                float z = 0.0;
                float w = 0.0;
                etype = UNIFORM_UNKNOWN;

                if (strncmp(type, "int", sizeof("int")) == 0) {
                    etype = UNIFORM_INT;
                    i = atoi(value);
                }

                if (strncmp(type, "float", sizeof("float")) == 0) {
                    etype = UNIFORM_FLOAT;
                    x = atof(value);
                }
                if (strncmp(type, "vec2", sizeof("vec2")) == 0) {
                    etype = UNIFORM_VEC2;
                    p = strtok(value, ",");
                    x = atof(p);
                    p = strtok(NULL, "\0");
                    y = atof(p);;
                }
                if (strncmp(type, "vec3", sizeof("vec3")) == 0) {
                    etype = UNIFORM_VEC3;
                    p = strtok(value, ",");
                    x = atof(p);
                    p = strtok(NULL, ",");
                    y = atof(p);;
                    p = strtok(NULL, "\0");
                    z = atof(p);;
                }

                if (strncmp(type, "vec4", sizeof("vec4")) == 0) {
                    etype = UNIFORM_VEC4;
                    p = strtok(value, ",");
                    x = atof(p);
                    p = strtok(NULL, ",");
                    y = atof(p);;
                    p = strtok(NULL, ",");
                    z = atof(p);;
                    p = strtok(NULL, "\0");
                    w = atof(p);;
                }

                UniformConfig *uconfig;
                if (current_section == VERT_SECTION) {
                    assert(c->vert_uniform_count + 1 != MAX_UNIFORM_COUNT);
                    uconfig = &(c->vert_uniforms[c->vert_uniform_count]);
                    c->vert_uniform_count++;
                } else {
                    assert(c->frag_uniform_count + 1 != MAX_UNIFORM_COUNT);
                    uconfig = &(c->frag_uniforms[c->frag_uniform_count]);
                    c->frag_uniform_count++;
                }

                uconfig->type = etype;
                strncpy(uconfig->name, name, MAX_LINE_LEN);
                uconfig->i = i;
                uconfig->x = x;
                uconfig->y = y;
                uconfig->z = z;
                uconfig->w = w;
            }
        }
    }
}

void destroy_config(Config *c)
{
    if (c == NULL) {
        return;
    }
    _destroy_app_config(&c->app);
    _destroy_object_config(&c->object);
    for (int i = 0; i < MAX_TEXTURE_COUNT; i++) {
        _destroy_texture_config(&c->textures[i]);
    }
    for (int i = 0; i < MAX_UNIFORM_COUNT; i++) {
        _destroy_uniform_config(&c->vert_uniforms[i]);
    }

    for (int i = 0; i < MAX_UNIFORM_COUNT; i++) {
        _destroy_uniform_config(&c->frag_uniforms[i]);
    }

    _set_config_defaults(c);
}

void _set_config_defaults(Config *c)
{
    assert(c != NULL);

    c->vert_uniform_count = CONFIG_DEFAULT_VERT_UNIFORM_COUNT;
    c->frag_uniform_count = CONFIG_DEFAULT_FRAG_UNIFORM_COUNT;
    c->texture_count = CONFIG_DEFAULT_TEXTURE_COUNT;
}

void print_config(Config *config)
{
    assert(config != NULL);

    printf("Config:\n");
    print_app_config(&config->app);
    print_object_config(&config->object);
    printf("texture_count: %d\n", config->texture_count);
    for (int i = 0; i < config->texture_count; i++) {
        print_texture_config(&(config->textures[i]));
    }
    printf("vert_uniform_count: %d\n", config->vert_uniform_count);
    for (int i = 0; i < config->vert_uniform_count; i++) {
        print_uniform_config(&(config->vert_uniforms[i]));
    }
    printf("frag_uniform_count: %d\n", config->frag_uniform_count);
    for (int i = 0; i < config->frag_uniform_count; i++) {
        print_uniform_config(&(config->frag_uniforms[i]));
    }
}

void print_app_config(AppConfig *config)
{
    assert(config != NULL);

    printf("AppConfig:\n");
    printf("  object_file - %s\n", config->object_filename);

}

void print_object_config(ObjectConfig *config)
{
    assert(config != NULL);

    printf("ObjectConfig:\n");
    printf("  vert_shader_file - %s\n", config->vert_shader_filename);
    printf("  frag_shader_file - %s\n", config->frag_shader_filename);

}

void print_uniform_config(UniformConfig *config)
{
    assert(config != NULL);

    printf("UniformConfig:\n");
    printf("  type - %d\n", config->type);
    printf("  name - %s\n", config->name);
    switch(config->type) {
    case UNIFORM_INT:
        printf("  i - %d\n", config->i);
        break;
    case UNIFORM_FLOAT:
        printf("  x - %f\n", config->x);
        break;
    case UNIFORM_VEC2:
        printf("  x - %f\n", config->x);
        printf("  y - %f\n", config->y);
        break;
    case UNIFORM_VEC3:
        printf("  x - %f\n", config->x);
        printf("  y - %f\n", config->y);
        printf("  z - %f\n", config->z);
        break;
    case UNIFORM_VEC4:
        printf("  x - %f\n", config->x);
        printf("  y - %f\n", config->y);
        printf("  z - %f\n", config->z);
        printf("  w - %f\n", config->w);
        break;
    default:
        printf("  UNKNOWN DATA TYPE\n");
        break;
    }
}

void print_texture_config(TextureConfig *config)
{
    assert(config != NULL);

    printf("TextureConfig:");
    printf("  name - %s\n", config->name);
    printf("  bmp_file - %s\n", config->filename);
}

void _init_app_config(AppConfig *c)
{
    assert(c != NULL);

    _set_app_config_defaults(c);
}

void _destroy_app_config(AppConfig *c)
{
    assert(c != NULL);

    _set_app_config_defaults(c);
}

void _set_app_config_defaults(AppConfig *c)
{
    assert(c != NULL);

    c->object_filename[0] = '\0';
}

void _init_object_config(ObjectConfig *c)
{
    assert(c != NULL);

    _set_object_config_defaults(c);
}

void _destroy_object_config(ObjectConfig *c)
{
    assert(c != NULL);

    _set_object_config_defaults(c);
}

void _set_object_config_defaults(ObjectConfig *c)
{
    assert(c != NULL);

    c->vert_shader_filename[0] = '\0';
    c->frag_shader_filename[0] = '\0';
}


void _init_texture_config(TextureConfig *c)
{
    assert(c != NULL);

    _set_texture_config_defaults(c);
}

void _destroy_texture_config(TextureConfig *c)
{
    assert(c != NULL);

    _set_texture_config_defaults(c);
}

void _set_texture_config_defaults(TextureConfig *c)
{
    assert(c != NULL);

    c->name[0] = '\0';
    c->filename[0] = '\0';
}

void _init_uniform_config(UniformConfig *c)
{
    assert(c != NULL);

    _set_uniform_config_defaults(c);
}

void _destroy_uniform_config(UniformConfig *c)
{
    assert(c != NULL);

    _set_uniform_config_defaults(c);
}

void _set_uniform_config_defaults(UniformConfig *c)
{
    assert(c != NULL);

    c->type = UNIFORM_UNKNOWN;
    c->name[0] = '\0';
    c->i = 0;
    c->x = 0.0;
    c->y = 0.0;
    c->z = 0.0;
    c->w = 0.0;
}
