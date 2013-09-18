#version 150
 
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 rot_x, rot_y, rot_z;
 
in vec4 position;
in vec3 color;
 
out vec3 Color;
 
void main()
{
    Color = color;
    vec3 p = vec3(position.xyz) * rot_x * rot_y * rot_z;
    gl_Position = projMatrix * viewMatrix * vec4(p, 1.0);
}
