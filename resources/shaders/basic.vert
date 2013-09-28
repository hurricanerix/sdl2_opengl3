#version 150
 
uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 RotMatrix;
 
in vec4 MCvertex;
in vec3 MCnormal;
 
void main()
{
    gl_Position = MVPMatrix * vec4(vec3(MCvertex.xyz) * RotMatrix, 1.0);
}
