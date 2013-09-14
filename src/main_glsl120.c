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


SDL_Window* display_window;
SDL_Renderer* display_renderer;
GLuint programID;
GLuint vertexbuffer;

static GLfloat vertices[] = {
     1.0, 1.0, 1.0,    // x == 1 face
     1.0, -1.0, 1.0,
     1.0, -1.0, -1.0,
     1.0, 1.0, -1.0,
    -1.0, 1.0, 1.0,    // x == -1 face
    -1.0, 1.0, -1.0,
    -1.0, -1.0, -1.0,
    -1.0, -1.0, 1.0,
     1.0, 1.0, 1.0,    // y == 1 face
     1.0, 1.0, -1.0,
    -1.0, 1.0, -1.0,
    -1.0, 1.0, 1.0,
     1.0, -1.0, 1.0,   // y == -1 face
    -1.0, -1.0, 1.0,
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0, 1.0, 1.0,    // z == 1 face
    -1.0, 1.0, 1.0,
    -1.0, -1.0, 1.0,
     1.0, -1.0, 1.0,
     1.0, 1.0, -1.0,   // z == -1 face
     1.0, -1.0, -1.0,
    -1.0, -1.0, -1.0,
    -1.0, 1.0, -1.0};

static GLfloat colors[] = {
    1.0, 0.0, 0.0,      // x == 1 face color
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,      // x == -1 face color
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,      // y == 1 face color
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,      // y == -1 face color
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,      // z == 1 face color
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,      // z == -1 face color
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0};

void init_gl() {
    printf("-- OpenGL Information --\n");
    printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(
        GL_SHADING_LANGUAGE_VERSION));

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glShadeModel (GL_FLAT);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    programID = load_shaders();
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
    glUseProgram(programID);

    return 1;
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int i,j;

    for ( i = 0; i < 6; i++ ) {
        glBegin(GL_POLYGON);
            for ( j = 0; j < 4; j++ ) {
                glArrayElement(4*i+j);
            }
        glEnd();
    }

    glFlush ();
    SDL_RenderPresent(display_renderer);
}

int main(int argc, char *argv[]) {
    init_sdl(&display_window, &display_renderer);
    init_gl();
    resize_viewport(800, 600);

    SDL_Event event;

    for (;;)
    {
        SDL_WaitEvent(&event);
        render();

        switch (event.type)
        {
        case SDL_QUIT:
            return;
        default:
            break;
        }
    }

    SDL_Delay(5000);
    SDL_Quit();

    return 0;
}
