#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

layout (location = 0) out vec2 TexCoord;
layout (location = 1) out vec3 LightDir;
layout (location = 2) out vec3 ViewDir;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform struct LightInfo {
 vec4 Position; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} Light;

void main() {
	// Transform normal and tangent to eye space
	vec3 norm = normalize(NormalMatrix * VertexNormal);
	vec3 tang = normalize(NormalMatrix * vec3(VertexTangent));

	// Compute the binormal
	vec3 binormal = normalize(cross(norm, tang)) * VertexTangent.w;

	// Matrix for transformation to tangent space
	mat3 toObjectLocal = mat3(
	tang.x, binormal.x, norm.x,
	tang.y, binormal.y, norm.y,
	tang.z, binormal.z, norm.z);

    vec3 pos = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;

	LightDir = toObjectLocal *  normalize(Light.Position.xyz - pos);
	ViewDir = toObjectLocal * normalize(-pos);

	TexCoord = VertexTexCoord;

	gl_Position = MVP * vec4(VertexPosition, 1.0);
}