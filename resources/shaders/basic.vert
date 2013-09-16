#version 140
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

uniform Transformation {
    mat4 projection_matrix;
    mat4 modelview_matrix;
};

in vec3 vertex;

void main(void) {
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(vertex, 1.0);
}
