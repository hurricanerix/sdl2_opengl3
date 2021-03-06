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


SDL_Window* display_window;
SDL_GLContext glcontext;

int app_running;

vec3 rotation;

Config *g_config;
Shader g_shader;
Object g_object;

mat4 proj_matrix;
mat4 view_matrix;
mat4 mvp_matrix;
mat3 rotation_matrix;

void render_scene();

void setup_textures(Config *config);
void cleanup(Config *config);
void process_keys(unsigned char key);
void set_uniforms();
void resize_display(int width, int height);


void load_app(Config *config)
{
    assert(config != NULL);

    app_running = TRUE;
    g_config = config;

    printf("Initializing SDL...");
    SDL_Init(SDL_INIT_EVERYTHING);
    printf(" Complete\n");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    display_window = SDL_CreateWindow("SDL2/OpenGL3 Demo",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
            SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    if (display_window == NULL) {
        set_error_msg(&config->status, "Could not create window.\n");
        return;
    }

    // Create an OpenGL context associated with the window.
    glcontext = SDL_GL_CreateContext(display_window);

    printf("glcontext: %p\n", glcontext);

    SDL_GL_SetSwapInterval(1);

    print_opengl_info();

    resize_display(800, 600);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.2, 0.2, 0.2, 1.0);

    init_shader(&g_shader);
    load_shader(&g_shader, config->object.vert_shader_filename,
        config->object.frag_shader_filename);
    if (g_shader.status.is_error) {
        copy_status(&(config->status), &(g_shader.status));
        return;
    }

    //setup_textures(config);
    Texture *colormap = NULL;

    if (config->texture_count > 0) {
        Texture tmp_colormap;
        init_texture(&tmp_colormap);
        load_texture(&tmp_colormap, config->textures[0].filename);
        colormap = &tmp_colormap;
    }

    g_object = init_object();
    load_object(&g_object, config->app.object_filename, &g_shader, colormap, NULL);
    if (g_object.status.is_error) {
        copy_status(&(config->status), &(g_object.status));
        return;
    }

    bind_object(&g_object);

    rotation.x = 0.0;
    rotation.y = 0.0;
    rotation.z = 0.0;
}

void run_app(Status *status)
{
    SDL_Event event;

    while (app_running)
    {
        SDL_PollEvent(&event);

        render_scene(g_config);

        switch (event.type)
        {
        case SDL_KEYDOWN:
            process_keys(event.key.keysym.sym);
            break;
        case SDL_QUIT:
            return;
            break;
        default:
            break;
        }
    }
}

void _print_renderer_info(FILE *fp, SDL_RendererInfo *info)
{
    assert(info != NULL);
    assert(fp != NULL);

    fprintf(fp, "Renderer Name: %s\n", info->name);
    fprintf(fp, "Renderer Flags: %d\n", info->flags);
    fprintf(fp, "Renderer Num Texture Formats: %d\n", info->num_texture_formats);
    for (int i = 0; i < info->num_texture_formats; i++) {
        fprintf(fp, "Renderer Texture Format[%d]: %d\n", i, info->texture_formats[i]);
    }
    fprintf(fp, "Renderer Max Texture Width: %d\n", info->max_texture_width);
    fprintf(fp, "Renderer Max Texture Height: %d\n", info->max_texture_height);
}

void _print_opengl_info(FILE *fp)
{
    assert(fp != NULL);

    fprintf(fp, "GL_VENDOR: %s\n", glGetString(GL_VENDOR));
    fprintf(fp, "GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    fprintf(fp, "GL_VERSION: %s\n", glGetString(GL_VERSION));
    fprintf(fp, "GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(
        GL_SHADING_LANGUAGE_VERSION));
}

void resize_display(int width, int height)
{
    if(height == 0) {
        height = 1;
    }

    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    float ratio;
    ratio = (1.0f * width) / height;

    proj_matrix = get_projection_matrix(53.13f, ratio, 1.0f, 30.0f);
}

void render_scene(Config *config)
{
    mvp_matrix = mult_mat4(proj_matrix, view_matrix);
    rotation_matrix = get_rotation_matrix(rotation);
    vec3 pos = {{.x=5}, {.y=5}, {.z=0}};
    vec3 lookAt = {{.x=0}, {.y=0}, {.z=0}};
    view_matrix = get_view_matrix(pos, lookAt);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(g_object.shader->program_id);
    set_uniforms(config);
    render_object(&g_object);

    SDL_GL_SwapWindow(display_window);
}

void process_keys(unsigned char key)
{
    switch (key) {
    case SDL_SCANCODE_LEFT:
        rotation.x += 0.01;
        break;
    case SDL_SCANCODE_RIGHT:
        rotation.x -= 0.01;
        break;
    case SDL_SCANCODE_UP:
        rotation.y += 0.01;
        break;
    case SDL_SCANCODE_DOWN:
        rotation.y -= 0.01;
        break;
    case SDLK_ESCAPE:
        app_running = FALSE;
        return;
        break;
    default:
        break;
    }
}

void set_uniforms(Config *config)
{
    // must be called after glUseProgram
    glUniformMatrix4fv(g_shader.proj_matrix_loc,  1, GL_FALSE, (float *)&mvp_matrix);
    glUniformMatrix4fv(g_shader.view_matrix_loc,  1, GL_FALSE, (float *)&view_matrix);
    glUniformMatrix3fv(g_shader.rot_matrix_loc,  1, GL_FALSE, (float *)&rotation_matrix);

    for (int i = 0; i < config->vert_uniform_count; i++) {
        bind_uniform(&g_shader, &(config->vert_uniforms[i]));
    }

    for (int i = 0; i < config->frag_uniform_count; i++) {
        bind_uniform(&g_shader, &(config->frag_uniforms[i]));
    }
}

void destroy_app()
{
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(display_window);
    SDL_Quit();
}


void setup_textures(Config *config)
{
    for(int i = 0; i < config->texture_count; i++) {
        //setup_texture(&(config->textures[i]));
    }
}
