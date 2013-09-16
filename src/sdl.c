#include <SDL2/SDL.h>

#include "sdl.h"

void init_sdl(SDL_Window **window, SDL_Renderer **renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_RendererInfo display_renderer_info;
    SDL_CreateWindowAndRenderer(
        800, 600, SDL_WINDOW_OPENGL, window, renderer);
    SDL_GetRendererInfo(*renderer, &display_renderer_info);
    if ((display_renderer_info.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (display_renderer_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        // TODO: Handle this. We have no render surface and not accelerated.
    }
    SDL_GL_SetSwapInterval(1);
}
