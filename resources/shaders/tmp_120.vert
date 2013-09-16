#version 120
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void) {
    vec4 MCVertex = gl_Vertex;
    gl_Position = ProjectionMatrix * ModelViewMatrix * MCVertex;
}
