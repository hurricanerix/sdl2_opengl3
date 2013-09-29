/*
 * From OpenGL Shading Language (3rd Edition)
 */
#version 150

const vec3 BrickColor = vec3(1.0, 0.3, 0.2);
const vec3 MortarColor = vec3(0.85, 0.86, 0.84);
const vec2 BrickSize = vec2(0.3, 0.15);
const vec2 BrickPct = vec2(0.9, 0.85);

in vec2    MCposition;
in float LightIntensity;

out vec4 FracColor;

void main()
{
    vec3 color;
    vec2 position, useBrick;

    position = MCposition / BrickSize;

    if (fract(position.y * 0.5) > 0.5)
        position.x += 0.5;

    position = fract(position);

    useBrick = step(position, BrickPct);

    color = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);
    color *= LightIntensity;
    FracColor = vec4(color, 1.0);
}
