#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define GL3_PROTOTYPES 1
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>

#include "sdl.h"
#include "shader.h"
#include "cube.h"


SDL_Window* display_window;
SDL_Renderer* display_renderer;
GLuint program_id;

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

    glShadeModel (GL_FLAT);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    if ((program_id = install_shader()) == -1) {
        fprintf(stderr, "ERROR(MAIN): could not load GLSL program.\n");
    }
}

int resize_viewport(int width, int height) {
    glViewport(0, 0, width, height);

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
    glTranslatef(0.0, 0.0, -5.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_FLAT);
    glUseProgram(program_id);

    return 1;
}

void render() {
   static GLfloat xrot, yrot, zrot;

   zrot += 3.0;
   xrot += 10.0;
   yrot += 7.0;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   glRotatef(xrot, 1.0, 0.0, 0.0);
   glRotatef(yrot, 0.0, 1.0, 0.0);
   glRotatef(zrot, 0.0, 0.0, 1.0);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   draw_cube();
   SDL_RenderPresent(display_renderer);
   glPopMatrix();
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
