/*
 * From OpenGL Shading Language (3rd Edition)
 */
#version 150

uniform vec3 LightPosition;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 RotMatrix;
// uniform mat3 NormalMatrix;

in vec4      MCvertex;
in vec3      MCnormal;
vec3 MCtangent = vec3(1.0, 0.0, 0.0);
in vec2      TexCoord0;

out vec3 LightDir;
out vec3 EyeDir;
out vec2 TexCoord;

// TODO: calculate this and pass it in.
mat3 getNormalMatrix()
{
    mat3 nm = mat3x3(MVMatrix);
    nm = inverse(nm);
    nm = transpose(nm);
    return nm;
}

void main()
{
    vec4 RMCvertex = vec4(MCvertex.xyz * RotMatrix, 1.0);
    vec3 RMCnormal = normalize(MCnormal * RotMatrix);
    gl_Position = MVPMatrix * RMCvertex;

    EyeDir = vec3(MVMatrix * MCvertex);
    TexCoord = TexCoord0.st;

    mat3 NormalMatrix = getNormalMatrix();
    vec3 n = normalize(NormalMatrix * RMCnormal);
    vec3 t = normalize(NormalMatrix * MCtangent);
    vec3 b = cross(n, t);

    vec3 v;
    v.x = dot(LightPosition, t);
    v.y = dot(LightPosition, b);
    v.z = dot(LightPosition, n);
    LightDir = normalize(v);

    v.x = dot(EyeDir, t);
    v.y = dot(EyeDir, b);
    v.z = dot(EyeDir, n);
    EyeDir = normalize(v);
}
