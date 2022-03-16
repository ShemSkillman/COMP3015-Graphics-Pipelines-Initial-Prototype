#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

layout (location = 0) out vec2 TexCoord;
layout (location = 1) out vec3 LightDir;
layout (location = 2) out vec3 ViewDir;
layout (location = 3) out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform struct LightInfo {
 vec4 Position; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} Light;

void main() {
    vec3 pos = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;

	LightDir = normalize(Light.Position.xyz - pos);
	ViewDir = normalize(-pos);

	TexCoord = VertexTexCoord;

	Normal = VertexNormal;

	gl_Position = MVP * vec4(VertexPosition, 1.0);
}