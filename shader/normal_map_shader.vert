#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

layout (location = 0) out vec4 Position;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec2 TexCoord;
layout (location = 3) out vec3 LightDir;
layout (location = 4) out vec3 ViewDir;


uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform struct SpotLightInfo {
vec4 Position; // Position in cam coords
vec3 L; // Diffuse/spec intensity
vec3 La; // Amb intensity
vec3 Direction; // Direction of the spotlight in cam coords.
float Exponent; // Angular attenuation exponent
float Cutoff; // Cutoff angle (between 0 and pi/2)
} Spot;

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

	Position = ModelViewMatrix * vec4(VertexPosition, 1.0);

	LightDir = toObjectLocal * Spot.Direction;
	ViewDir = toObjectLocal * normalize(-Position.xyz);

	TexCoord = VertexTexCoord;
	Normal = normalize(NormalMatrix * VertexNormal);

	gl_Position = MVP * vec4(VertexPosition, 1.0);
}