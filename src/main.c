#include <stdio.h>
#include <stdlib.h>

#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>

#include "logger.h"
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

void print_help(char *command) {
    fprintf(stderr, "%s help:\n", command);
    fprintf(stderr, "%s <ply file>\n\n", command);
}

void log_opengl_info(void)
{
    log_info("GL_VENDOR: %s", glGetString(GL_VENDOR));
    log_info("GL_RENDERER: %s", glGetString(GL_RENDERER));
    log_info("GL_VERSION: %s", glGetString(GL_VERSION));
    log_info("GL_SHADING_LANGUAGE_VERSION: %s", glGetString(
        GL_SHADING_LANGUAGE_VERSION));
}

void log_renderer_info(SDL_RendererInfo *ri)
{
    log_info("Renderer Name: %s", ri->name);
    log_info("Renderer Flags: %d", ri->flags);
    log_info("Renderer Num Texture Formats: %d", ri->num_texture_formats);
    for (int i = 0; i < ri->num_texture_formats; i++) {
        log_info("Renderer Texture Format[%d]: %d", i, ri->texture_formats[i]);
    }
    log_info("Renderer Max Texture Width: %d", ri->max_texture_width);
    log_info("Renderer Max Texture Height: %d", ri->max_texture_height);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        print_help(argv[0]);
        exit(1);
    }
    FILE *debug_out = NULL;
    if (argc == 3) {
        debug_out = stderr;
    }
    init_logger(debug_out, stdout, stderr, stderr);
    log_info("Starting Application");

    char *ply_file = argv[1];

    log_info("Initializing SDL...");
    SDL_Init(SDL_INIT_VIDEO);
    log_info("Initializing SDL Complete");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_RendererInfo display_renderer_info;
    log_info("Creating window and renderer...");
    SDL_CreateWindowAndRenderer(
        800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, &display_window, &display_renderer);
    log_info("Creating window and renderer Complete");

    SDL_GetRendererInfo(display_renderer, &display_renderer_info);
    if ((display_renderer_info.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (display_renderer_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        log_error("No render surface found");
        // TODO: Handle this. We have no render surface and not accelerated.
    }
    log_renderer_info(&display_renderer_info);

    SDL_GL_SetSwapInterval(1);

    log_opengl_info();

    changeSize(800, 600);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2,0.2,0.2,1.0);

    p = setupShaders(vertexFileName, fragmentFileName);
    setupBuffers(ply_file);

    SDL_Event event;

    for (;;)
    {
        SDL_PollEvent(&event);

        renderScene();

        switch (event.type)
        {
        case SDL_QUIT:
            log_info("terminating application");
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

    render_object();

    SDL_GL_SwapWindow(display_window);
}

void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27) {
        destroy_object();
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
