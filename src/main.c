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
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/gl3.h>
#endif
#include <SDL2/SDL.h>

#include "3dmath.h"
#include "config.h"
#include "logger.h"
#include "object.h"
#include "shader.h"
#include "text.h"
#include "main.h"


SDL_Window* display_window;
SDL_Renderer* display_renderer;

GLuint p,v,f;
float projMatrix[16];
float viewMatrix[16];
float mvpMatrix[16];
float rotMatrix[9];

void render_scene();
void setup_textures(Config *config);
void cleanup(Config *config);
void process_keys(unsigned char key, int x, int y);
void set_uniforms();
void change_size(int w, int h);

void print_help(char *command)
{
    assert(command != NULL);

    fprintf(stderr, "%s help:\n", command);
    fprintf(stderr, "%s <config file>\n\n", command);
}

void print_opengl_info()
{

    printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(
        GL_SHADING_LANGUAGE_VERSION));

}

void print_renderer_info(SDL_RendererInfo *ri)
{
    assert(ri != NULL);

    printf("Renderer Name: %s\n", ri->name);
    printf("Renderer Flags: %d\n", ri->flags);
    printf("Renderer Num Texture Formats: %d\n", ri->num_texture_formats);
    for (int i = 0; i < ri->num_texture_formats; i++) {
        printf("Renderer Texture Format[%d]: %d\n", i, ri->texture_formats[i]);
    }
    printf("Renderer Max Texture Width: %d\n", ri->max_texture_width);
    printf("Renderer Max Texture Height: %d\n", ri->max_texture_height);

}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        print_help(argv[0]);
        exit(1);
    }
    FILE *debug_out = NULL;
    if (argc == 3) {
        debug_out = stderr;
    }

    init_logger(stderr);
    for (int i = 0; i < argc; i++) {
    }

    Config *config = get_config(argv[1]);
    print_config(config);

    printf("Initializing SDL...\n");
    SDL_Init(SDL_INIT_VIDEO);
    printf("Initializing SDL Complete\n");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_RendererInfo display_renderer_info;
    printf("Creating window and renderer...\n");
    SDL_CreateWindowAndRenderer(
        800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, &display_window, &display_renderer);
    printf("Creating window and renderer Complete\n");

    SDL_GetRendererInfo(display_renderer, &display_renderer_info);
    if ((display_renderer_info.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (display_renderer_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        log_error("No render surface found");
        // TODO: Handle this. We have no render surface and not accelerated.
    }
    print_renderer_info(&display_renderer_info);

    SDL_GL_SetSwapInterval(1);

    print_opengl_info();

    change_size(800, 600);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2,0.2,0.2,1.0);

    p = setupShaders(config->object->vert_shader_file,
        config->object->frag_shader_file);
    setupBuffers(config->app->object_file);
    setup_textures(config);

    SDL_Event event;

    for (;;)
    {
        SDL_PollEvent(&event);

        render_scene(config);

        switch (event.type)
        {
        case SDL_QUIT:
            cleanup(config);
            return 0;
        default:
            break;
        }
    }

    SDL_Quit();

    return 0;
}

void render_scene(Config *config)
{
    copy_matrix(mvpMatrix, projMatrix);
    multMatrix(mvpMatrix, viewMatrix);
    print_matrix("model view projection matrix", mvpMatrix, 4);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static float x, y, z;
    x += 0.02;
    y += 0.05;
    z += 0.01;

    get_rot_matrix(rotMatrix, x, y, z);
    print_matrix("rotation matrix", rotMatrix, 3);

    //setCamera(viewMatrix, 10, 2, 10, 0, 2, -5);
    setCamera(viewMatrix, 5, 5, 0, 0, 0, 0);
    print_matrix("view matrix", viewMatrix, 4);
    glUseProgram(p);
    set_uniforms(config);

    render_object();

    SDL_GL_SwapWindow(display_window);

}

void cleanup(Config *config)
{

    destroy_config(config);

}

void process_keys(unsigned char key, int x, int y)
{

    if (key == 27) {
        destroy_object();
        glDeleteProgram(p);
        //glDeleteShader(v);
        //glDeleteShader(f);
        exit(0);
    }

}

void set_uniforms(Config *config)
{

    // must be called after glUseProgram
    glUniformMatrix4fv(projMatrixLoc,  1, GL_FALSE, mvpMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, GL_FALSE, viewMatrix);
    glUniformMatrix3fv(rotMatrixLoc,  1, GL_FALSE, rotMatrix);

    for (int i = 0; i < config->vert_uniform_count; i++) {
        set_uniform(p, &(config->vert_uniforms[i]));
    }

    for (int i = 0; i < config->frag_uniform_count; i++) {
        set_uniform(p, &(config->frag_uniforms[i]));
    }


}


void change_size(int w, int h)
{

    float ratio;
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    ratio = (1.0f * w) / h;
    buildProjectionMatrix(projMatrix, 53.13f, ratio, 1.0f, 30.0f);
    print_matrix("projection matrix", projMatrix, 4);
}

void setup_textures(Config *config)
{

    for(int i = 0; i < config->texture_count; i++) {
        setup_texture(&(config->textures[i]));
    }

}
