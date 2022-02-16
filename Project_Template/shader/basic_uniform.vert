#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform vec3 Ka;
uniform vec3 La;
uniform vec4 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform int SpecularPowerCoefficient;
uniform vec3 ViewPosition;
uniform vec3 Ks;
uniform vec3 Ls;

out vec3 LightIntensity;

void main()
{
    vec3 n = normalize(VertexNormal * NormalMatrix);

	vec4 vertexPos = vec4(VertexPosition, 1.0) * ModelViewMatrix;

	vec3 s = vec3(normalize(LightPosition - vertexPos));

	vec3 r = reflect(-s, n);

	LightIntensity = (Ka * La) + (Kd * Ld * max(dot(s, n), 0.0f)) + (Ks * Ls * pow(max(dot(r, ViewPosition), 0.0f), SpecularPowerCoefficient));

    gl_Position = vec4(VertexPosition,1.0) * MVP;
}