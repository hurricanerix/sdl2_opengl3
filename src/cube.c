#include <OpenGL/gl3.h>
#include "cube.h"


/*
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
*/

GLuint triangleVBO;
const float NUM_OF_VERTICES_IN_DATA=3;
float data[3][3] = {
    {  0.0, 1.0, 0.0   },
    { -1.0, -1.0, 0.0  },
    {  1.0, -1.0, 0.0  }
    };

void create_cube_buffer() {
    const unsigned int shaderAttribute = 0;

    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, NUM_OF_VERTICES_IN_DATA * 3 * sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
}

void draw_cube() {
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
