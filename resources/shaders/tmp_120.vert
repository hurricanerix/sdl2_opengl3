#version 120
//uniform mat4 ModelViewMatrix;
//uniform mat4 ProjectionMatrix;

void main(void) {
    mat4 ModelViewMatrix = gl_ModelViewMatrix;
    mat4 ProjectionMatrix = gl_ProjectionMatrix;
    vec4 MCVertex = gl_Vertex;

    gl_Position = ProjectionMatrix * ModelViewMatrix * MCVertex;
}
