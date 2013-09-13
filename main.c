#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>

#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>



SDL_Window* displayWindow;
SDL_Renderer* displayRenderer;

void Display_InitGL() {
    /* Enable smooth shading */
    //glShadeModel( GL_SMOOTH );

    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    /* Depth buffer setup */
    glClearDepth( 1.0f );

    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );

    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL );

    /* Really Nice Perspective Calculations */
    //glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

}

/* function to reset our viewport after a window resize */
int Display_SetViewport( int width, int height ) {
    /* Height / width ration */
    GLfloat ratio;

    /* Protect against a divide by zero */
    if ( height == 0 ) {
        height = 1;
    }

    ratio = ( GLfloat )width / ( GLfloat )height;

    /* Setup our viewport. */
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

    /* change to the projection matrix and set our viewing volume. */
    //glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );

    /* Make sure we're chaning the model view and not the projection */
    //glMatrixMode( GL_MODELVIEW );

    /* Reset The View */
    glLoadIdentity( );

    return 1;
}

void Display_Render() {
    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();
    glTranslatef( -1.5f, 0.0f, -6.0f );

    glBegin( GL_TRIANGLES );            /* Drawing Using Triangles */
      glVertex3f(  0.0f,  1.0f, 0.0f ); /* Top */
      glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
      glVertex3f(  1.0f, -1.0f, 0.0f ); /* Bottom Right */
    glEnd( );                           /* Finished Drawing The Triangle */

    /* Move Right 3 Units */
    glTranslatef( 3.0f, 0.0f, 0.0f );

    /*
    glBegin( GL_QUADS );
      glVertex3f( -1.0f,  1.0f, 0.0f );
      glVertex3f(  1.0f,  1.0f, 0.0f );
      glVertex3f(  1.0f, -1.0f, 0.0f );
      glVertex3f( -1.0f, -1.0f, 0.0f );
    glEnd( );
    */

    SDL_RenderPresent(displayRenderer);
    SDL_GL_SetSwapInterval(1);
}

int main(int argc, char *argv[]) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_RendererInfo displayRendererInfo;
    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
    SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
    /*TODO: Check that we have OpenGL */
    if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        /*TODO: Handle this. We have no render surface and not accelerated. */
    }



    Display_InitGL();

    Display_SetViewport(800, 600);

    Display_Render();

    printf("-- OpenGL Information --\n");
    printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    SDL_Delay(5000);
    SDL_Quit();

    return 0;
}
