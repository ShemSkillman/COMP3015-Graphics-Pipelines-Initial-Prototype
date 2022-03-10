#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

layout (location = 0) out vec3 EyeNormal;
layout (location = 1) out vec4 EyePosition;
layout (location = 2) out vec4 ProjTexCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelMatrix;
uniform mat4 MVP;
uniform mat4 ProjectorMatrix;

void main() { 
	vec4 pos4 = vec4(VertexPosition,1.0);

	EyeNormal = normalize(NormalMatrix * VertexNormal);
	EyePosition = ModelViewMatrix * vec4(VertexPosition, 1.0);

	ProjTexCoord = ProjectorMatrix * (ModelMatrix * pos4);

	gl_Position = MVP * vec4(VertexPosition, 1.0);
}