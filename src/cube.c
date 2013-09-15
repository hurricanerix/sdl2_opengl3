#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

#include "cube.h"


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

void create_cube_buffer() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
}

void draw_cube() {
    int i,j;

    for ( i = 0; i < 6; i++ ) {
        glBegin(GL_POLYGON);
            for ( j = 0; j < 4; j++ ) {
                glArrayElement(4*i+j);
            }
        glEnd();
    }

    glFlush ();
}
