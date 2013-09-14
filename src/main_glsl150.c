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


/*
char vert_shader_src[] = 
    "#version 120\n"
    "void main(void) {"
    "    gl_Position = ftransform();"
    "}";

char frag_shader_src[] = 
    "#version 120\n"
    "void main(void) {"
    "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
    "}";
*/

float p_matrix[] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 0
};

float mv_matrix[] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 0
};

/*
char vert_shader_src[] =
    "#version 150\n"
    "uniform mat4 ModelViewMatrix;"
    "uniform mat4 ProjectionMatrix;"
    "uniform Transformation {"
    "    mat4 projection_matrix;"
    "    mat4 modelview_matrix;"
    "};"
    "in vec3 vertex;"
    "void main(void) {"
    "//    gl_Position = projection_matrix * modelview_matrix * vec4(vertex, 1.0);"
    "    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(vertex, 1.0);"
    "}";
*/
int prim = GL_POLYGON;		/* GL primitive to draw */

  static GLfloat vertices[] = {
                                1.0, 1.0, 1.0,    // x == 1 face
                                1.0, -1.0, 1.0,
                                1.0, -1.0, -1.0,
                                1.0, 1.0, -1.0,
                                -1.0, 1.0, 1.0,   // x == -1 face
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
                                -1.0, 1.0, -1.0
                              };
  static GLfloat colors[] = {
                              1.0, 0.0, 0.0,      // x == 1 face color
                              1.0, 0.0, 0.0,      // x == 1 face color
                              1.0, 0.0, 0.0,      // x == 1 face color
                              1.0, 0.0, 0.0,      // x == 1 face color
                              1.0, 0.0, 0.0,      // x == -1 face color
                              1.0, 0.0, 0.0,      // x == -1 face color
                              1.0, 0.0, 0.0,      // x == -1 face color
                              1.0, 0.0, 0.0,      // x == -1 face color
                              0.0, 1.0, 0.0,      // y == 1 face color
                              0.0, 1.0, 0.0,      // y == 1 face color
                              0.0, 1.0, 0.0,      // y == 1 face color
                              0.0, 1.0, 0.0,      // y == 1 face color
                              0.0, 1.0, 0.0,      // y == -1 face color
                              0.0, 1.0, 0.0,      // y == -1 face color
                              0.0, 1.0, 0.0,      // y == -1 face color
                              0.0, 1.0, 0.0,      // y == -1 face color
                              0.0, 0.0, 1.0,      // z == 1 face color
                              0.0, 0.0, 1.0,      // z == 1 face color
                              0.0, 0.0, 1.0,      // z == 1 face color
                              0.0, 0.0, 1.0,      // z == 1 face color
                              0.0, 0.0, 1.0,      // z == -1 face color
                              0.0, 0.0, 1.0,      // z == -1 face color
                              0.0, 0.0, 1.0,      // z == -1 face color
                              0.0, 0.0, 1.0       // z == -1 face color
                            };

#define FRUSTUM_TOP (-1.0)
#define FRUSTUM_LEFT (1.0)
#define FRUSTUM_RIGHT (-1.0)
#define FRUSTUM_BOTTOM (1.0)
#define FRUSTUM_NEAR (1.0)
#define FRUSTUM_FAR (10.0)

static void setProjection( GLfloat pm[16] )
{
    int w = 640;
    int h = 480;

    pm[0] = (2.0 * FRUSTUM_NEAR) / (FRUSTUM_RIGHT - FRUSTUM_LEFT);
    pm[1] = 0.0; 
    pm[2] = 0.0; 
    pm[3] = 0.0;

    pm[4] = 0.0;
    pm[5] = (2.0 * FRUSTUM_NEAR) / (FRUSTUM_TOP - FRUSTUM_BOTTOM);
    pm[6] = 0.0;
    pm[7] = 0.0;

    pm[8] = (FRUSTUM_RIGHT + FRUSTUM_LEFT) / (FRUSTUM_RIGHT - FRUSTUM_LEFT);
    pm[9] = (FRUSTUM_TOP + FRUSTUM_BOTTOM) / (FRUSTUM_TOP - FRUSTUM_BOTTOM);
    pm[10] = -(FRUSTUM_FAR + FRUSTUM_NEAR) / (FRUSTUM_FAR - FRUSTUM_NEAR);
    pm[11] = -1.0; 

    pm[12] = 0.0;
    pm[13] = 0.0;
    pm[14] = -(2.0 * FRUSTUM_NEAR * FRUSTUM_FAR) / (FRUSTUM_FAR - FRUSTUM_NEAR);
    pm[15] = 0.0;
}

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

    GLint transformID = glGetUniformBlockIndex(programID, "Transformation");
    printf("Transform: %d\n", transformID);

    GLfloat modelViewMatrix[16];
    glGetFloatv( GL_MODELVIEW_MATRIX, modelViewMatrix );
    GLfloat projMatrix[16];
    setProjection( projMatrix );
    glGetFloatv( GL_MODELVIEW_MATRIX, projMatrix );

    GLuint projID = glGetUniformLocation(programID, "ProjectionMatrix"); 
    glUniformMatrix4fv(projID, 1, GL_FALSE, p_matrix);

    GLuint mvID = glGetUniformLocation(programID, "ModelViewMatrix"); 
    glUniformMatrix4fv(mvID, 1, GL_FALSE, mv_matrix);
    //GLint p_offset = glGetUniformBlockiv(programID, transformID, "projection_matrix", GL_UNIFORM_BLOCK_BINDING);
    //printf("P-offset: %d\n", p_offset);
    //GLint mv_matrixID = glGetUniformLocation(programID, "modelview_matrix");
    //printf("MV-Matrix: %d\n", mv_matrixID);
/*
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Create and compile our GLSL program from the shaders


    //glEnableClientState(GL_VERTEX_ARRAY);
*/
}

int resize_viewport(int width, int height) {
    int w = width;
    int h = height;
   glViewport(0, 0, w, h);

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
/*
    glViewport(0,0, width, height); 

    glClearColor(0.0,0.0,0.0,0.0);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode( GL_MODELVIEW ); 
    glLoadIdentity();


    glClear( GL_COLOR_BUFFER_BIT );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    */
    return 1;
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //2glLoadIdentity();
    /*
    glBegin(GL_TRIANGLES);
    glColor3f(0.1, 0.2, 0.3);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 1, 0);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
     glVertex2f(-0.5, -0.5);
     glVertex2f(-0.5, 0.5);
     glVertex2f(0.5, 0.5);
     glVertex2f(0.5, -0.5);
    glEnd();
    glFlush();
    */

  int i,j;

  for ( i = 0; i < 6; i++ ) {
    glBegin(prim);
      for ( j = 0; j < 4; j++ ) {
        glArrayElement(4*i+j);
      }
    glEnd();
  }
    glFlush ();
/*

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    printf("test\n");
    glEnableClientState(GL_VERTEX_ARRAY);
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glBegin(GL_TRIANGLES);
    glArrayElement(2);
    glArrayElement(3);
    glArrayElement(5);
    glEnd();
    glDisableClientState(GL_VERTEX_ARRAY);
*/

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

    //glDisableClientState(GL_VERTEX_ARRAY);
    SDL_Quit();

    return 0;
}
