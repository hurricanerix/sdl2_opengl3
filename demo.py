import sys
import ctypes
from sdl2 import *
import OpenGL
OpenGL.ERROR_LOGGING = False
from OpenGL.GL import *


def main():
    SDL_Init(SDL_INIT_VIDEO)

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2)
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)

    window = SDL_CreateWindow(b"Hello World",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)

    windowsurface = SDL_GetWindowSurface(window)

    glcontext = SDL_GL_CreateContext(window)

    print "GL_VENDOR: {}".format(glGetString(GL_VENDOR))
    print "GL_RENDERER: {}".format(glGetString(GL_RENDERER))
    print "GL_VERSION: {}".format(glGetString(GL_VERSION))
    print "GL_SHADING_LANGUAGE_VERSION: {}".format(glGetString(
        GL_SHADING_LANGUAGE_VERSION))

    glEnable(GL_DEPTH_TEST)

    glClearColor(0.2, 0.2, 0.2, 1.0)

    #image = SDL_LoadBMP(b"exampleimage.bmp")
    #SDL_BlitSurface(image, None, windowsurface, None)

    #SDL_UpdateWindowSurface(window)
    #SDL_FreeSurface(image)

    running = True
    event = SDL_Event()
    while running:
        while SDL_PollEvent(ctypes.byref(event)) != 0:
            if event.type == SDL_QUIT:
                running = False
                break

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        SDL_GL_SwapWindow(window)

    SDL_DestroyWindow(window)
    SDL_Quit()
    return 0

if __name__ == "__main__":
    sys.exit(main())
