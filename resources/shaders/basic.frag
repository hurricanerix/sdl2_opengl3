#version 150
 
out vec4 FracColor;

in float LightIntensity;

void main()
{
    vec3 color = vec3(1.0, 0.0, 0.0) * LightIntensity;
    FracColor = vec4(color, 1.0);
}
