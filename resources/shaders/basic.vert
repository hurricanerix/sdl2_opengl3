#version 150
 
uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 RotMatrix;
 
in vec4      MCvertex;
in vec3      MCnormal;

const vec3   LightPosition = vec3(2.0, 2.0, 2.0);
const float  SpecularContribution = 0.3;
const float  DiffuseContribution = 1.0 - SpecularContribution;

out float    LightIntensity;

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
    mat3 NormalMatrix = getNormalMatrix();
    vec3 ecPosition   = vec3(MVMatrix * RMCvertex);
    vec3 tnorm        = normalize(NormalMatrix * RMCnormal);
    vec3 lightVec     = normalize(LightPosition - ecPosition);
    vec3 reflectVec   = reflect(-lightVec, tnorm);
    vec3 viewVec      = normalize(-ecPosition);
    float diffuse     = max(dot(lightVec, tnorm), 0.0);
    float spec        = 0.0;

    if (diffuse > 0.0) {
        spec = max(dot(reflectVec, viewVec), 0.0);
        spec = pow(spec, 16.0);
    }

    LightIntensity = DiffuseContribution * diffuse +
                     SpecularContribution * spec;

    gl_Position = MVPMatrix * RMCvertex;
}
