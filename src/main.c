#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>

#include "sdl.h"
#include "shader.h"
#include "cube.h"


SDL_Window* display_window;
SDL_Renderer* display_renderer;
GLuint program_id;

GLfloat model_view_matrix[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f};

GLfloat model_view_proj_matrix[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.222222f, -1.0f,
    0.0f, 0.0f, 3.888889f, 5.0f};

/*
X Rotation:

[1 0 0 0]
[0 cos(-X Angle) -sin(-X Angle) 0]
[0 sin(-X Angle) cos(-X Angle) 0]
[0 0 0 1]
Y Rotation:

[cos(-Y Angle) 0 sin(-Y Angle) 0]
[0 1 0 0]
[-sin(-Y Angle) 0 cos(-Y Angle) 0]
[0 0 0 1]
Z Rotation:

[cos(-Z Angle) -sin(-Z Angle) 0 0]
[sin(-Z Angle) cos(-Z Angle) 0 0]
[0 0 1 0]
[0 0 0 1]
*/

void init_gl() {
    printf("-- OpenGL Information --\n");
    printf("---{info-start}---\n");
    printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(
        GL_SHADING_LANGUAGE_VERSION));
    printf("---{info-stop}---\n");

    create_cube_buffer();

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    if ((program_id = install_shader()) == -1) {
        fprintf(stderr, "ERROR(MAIN): could not load GLSL program.\n");
    }
}

int resize_viewport(int width, int height) {
    glViewport(0, 0, width, height);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glUseProgram(program_id);

    int model_view_matrix_location = glGetUniformLocation(
        program_id, "ModelViewMatrix");
    int model_view_proj_matrix_location = glGetUniformLocation(
        program_id, "ProjectionMatrix");
    glUniformMatrix4fv(
        model_view_matrix_location, 1, GL_FALSE, model_view_matrix);
    glUniformMatrix4fv(
        model_view_proj_matrix_location, 1, GL_FALSE, model_view_proj_matrix);

    return 1;
}

void render() {
   static GLfloat xrot, yrot, zrot;

   zrot += 3.0;
   xrot += 10.0;
   yrot += 7.0;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   draw_cube();
   SDL_RenderPresent(display_renderer);
   glFinish();
}

int main(int argc, char *argv[]) {
    init_sdl(&display_window, &display_renderer);
    init_gl();
    resize_viewport(800, 600);

    SDL_Event event;

    for (;;)
    {
        SDL_PollEvent(&event);

        render();

        switch (event.type)
        {
        case SDL_QUIT:
            return 0;
        default:
            break;
        }
    }

    SDL_Quit();

    return 0;
}
