#version 150
in vec3 vertex;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void) {
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(vertex, 1.0);
}
