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
#include "config.h"

#include "logger.h"
#include "main.h"
#include "text.h"


const int max_line_len = 1024;
enum ConfigSections {
    UNKNOWN_SECTION, APP_SECTION, OBJECT_SECTION
};
#define SECTION_START_TOKEN ('[')
#define SECTION_END_TOKEN (']')
#define COMMENT_TOKEN (';')
#define KEY_VALUE_DELIMITER ('=')
#define SPACE_TOKEN (' ')
#define EMPTY_LINE_TOKEN ('\0')

#define CUBE_FILE ("resources/objects/cube.ply\0")
#define VERT_FILE ("resources/shaders/basic.vert\0")
#define FRAG_FILE ("resources/shaders/basic.frag\0")


Config *get_config(char *filename)
{
    assert(filename != NULL);
    log_debug("read_config {");
    log_debug("  -in- filename - %s", filename);

    Config *config = malloc(sizeof(Config));
    assert(config != NULL);

    config->app = malloc(sizeof(AppConfig));
    assert(config->app != NULL);
    config->app->object_file = NULL;

    config->object = malloc(sizeof(ObjectConfig));
    assert(config->object != NULL);
    config->object->vert_shader_file = NULL;
    config->object->frag_shader_file = NULL;

    int ini_size;
    char *ini_data = text_file_read(filename, &ini_size);
    assert(ini_data != NULL);

    char buffer[max_line_len];
    enum ConfigSections current_section = UNKNOWN_SECTION;
    char *key;
    char *value;
    while (get_next_line(ini_data, ini_size, buffer, max_line_len) == TRUE) {
        switch(buffer[0]) {
        case COMMENT_TOKEN:
        case EMPTY_LINE_TOKEN:
        case SPACE_TOKEN:
            // Ignore these.
            break;
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

            break;
        default:
            if (current_section != APP_SECTION &&
                    current_section != OBJECT_SECTION) {
                continue;
            }

            key = strtok(&buffer[0], "=");
            assert(key != NULL);

            value = strtok(NULL, "\0");
            assert(value != NULL);

            if (current_section == APP_SECTION) {
                if (strncmp(key, "object_file", sizeof("object_file")) == 0) {
                    config->app->object_file = malloc(max_line_len);
                    assert(config->app->object_file != NULL);
                    strncpy(config->app->object_file, value, max_line_len);
                }
            }

            if (current_section == OBJECT_SECTION) {
                if (strncmp(key, "vert_shader_file", sizeof("vert_shader_file")) == 0) {
                    config->object->vert_shader_file = malloc(max_line_len);
                    assert(config->object->vert_shader_file != NULL);
                    strncpy(config->object->vert_shader_file, value, max_line_len);
                }
                else if (strncmp(key, "frag_shader_file", sizeof("frag_shader_file")) == 0) {
                    config->object->frag_shader_file = malloc(max_line_len);
                    assert(config->object->frag_shader_file != NULL);
                    strncpy(config->object->frag_shader_file, value, max_line_len);
                }
            }
        }
    }

    log_debug("read_config }");
    log_debug("  -out- config - %x", config);
    return config;
}

void log_config(Config *config)
{
    assert(config != NULL);
    log_debug("log_config {");
    log_debug("  -in- config - %x", config);

    log_info("Config:");
    log_debug("  app - %x", config->app);
    log_app_config(config->app);
    log_debug("  object - %x", config->object);
    log_object_config(config->object);

    log_debug("log_config }");
}

void log_app_config(AppConfig *config)
{
    assert(config != NULL);
    log_debug("log_app_config {");
    log_debug("  -in- config - %x", config);

    log_info("AppConfig:");
    log_info("  object_file - %s", config->object_file);

    log_debug("log_app_config }");
}

void log_object_config(ObjectConfig *config)
{
    assert(config != NULL);
    log_debug("log_object_config {");
    log_debug("  -in- config - %x", config);

    log_info("ObjectConfig:");
    log_info("  vert_shader_file - %s", config->vert_shader_file);
    log_info("  frag_shader_file - %s", config->frag_shader_file);

    log_debug("log_object_config }");
}

void destroy_config(Config *config)
{
    assert(config != NULL);
    log_debug("destroy_config {");

    assert(config->app != NULL);
    free(config->app->object_file);
    free(config->app);

    assert(config->object != NULL);
    free(config->object->vert_shader_file);
    free(config->object->frag_shader_file);
    free(config->object);

    free(config);

    log_debug("destroy_config }");
}
