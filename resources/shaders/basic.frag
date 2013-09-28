#version 150
 
out vec4 FracColor;

const vec3 color = vec3(1.0, 0.0, 0.0);
 
void main()
{
    FracColor = vec4(color, 1.0);
}
