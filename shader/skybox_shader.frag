#version 430

layout (location = 0) in vec4 Position;

layout(binding=0) uniform samplerCube SkyBoxTex;

layout (location = 0) out vec4 FragColor;

void main()
{
	vec3 texColor = texture(SkyBoxTex, normalize(Position.xyz)).rgb;

	FragColor = vec4(texColor, 1.0f); // final colour
}