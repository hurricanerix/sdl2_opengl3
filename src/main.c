#include <stdio.h>
#include <stdlib.h>

#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>

#include "3dmath.h"
#include "text.h"
#include "shader.h"
#include "object.h"

SDL_Window* display_window;
SDL_Renderer* display_renderer;
float rotMatrix[9];
char *vertexFileName = "resources/shaders/basic.vert";
char *fragmentFileName = "resources/shaders/basic.frag";
GLuint p,v,f;
float projMatrix[16];
float viewMatrix[16];


void renderScene(void);
void processNormalKeys(unsigned char key, int x, int y);
void setUniforms();
void changeSize(int w, int h);


int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_RendererInfo display_renderer_info;
    SDL_CreateWindowAndRenderer(
        800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, &display_window, &display_renderer);
    SDL_GetRendererInfo(display_renderer, &display_renderer_info);
    if ((display_renderer_info.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (display_renderer_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        // TODO: Handle this. We have no render surface and not accelerated.
    }
    SDL_GL_SetSwapInterval(1);

    printf("-- OpenGL Information --\n");
    printf("---{info-start}---\n");
    printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(
        GL_SHADING_LANGUAGE_VERSION));
    printf("---{info-stop}---\n");

    changeSize(800, 600);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2,0.2,0.2,1.0);

    p = setupShaders(vertexFileName, fragmentFileName);
    setupBuffers();

    SDL_Event event;

    for (;;)
    {
        SDL_PollEvent(&event);

        renderScene();

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

void renderScene(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static float x, y, z;
    x += 0.02;
    y += 0.05;
    z += 0.01;

    get_rot_matrix(rotMatrix, x, y, z);

    setCamera(viewMatrix, 10, 2, 10, 0, 2, -5);
    glUseProgram(p);
    setUniforms();

    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, triangle_count);

    SDL_GL_SwapWindow(display_window);
}

void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27) {
        glDeleteVertexArrays(1,vao);
        glDeleteProgram(p);
        //glDeleteShader(v);
        //glDeleteShader(f);
        exit(0);
    }
}

void setUniforms() {
    // must be called after glUseProgram
    glUniformMatrix4fv(projMatrixLoc,  1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, GL_FALSE, viewMatrix);
    glUniformMatrix3fv(rotMatrixLoc,  1, GL_FALSE, rotMatrix);
}


void changeSize(int w, int h) {

    float ratio;
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    ratio = (1.0f * w) / h;
    buildProjectionMatrix(projMatrix, 53.13f, ratio, 1.0f, 30.0f);
}
