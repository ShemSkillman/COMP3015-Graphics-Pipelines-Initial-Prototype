#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform vec4 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

out vec3 LightIntensity;

void main()
{
    vec3 n = normalize(VertexNormal * NormalMatrix );

	vec4 pos = vec4(VertexPosition, 1.0) * ModelViewMatrix;

	vec3 s = vec3(normalize(LightPosition - pos));

	LightIntensity = Kd * Ld * max(dot(s, n), 0.3f);

    gl_Position = vec4(VertexPosition,1.0) * MVP;
}
