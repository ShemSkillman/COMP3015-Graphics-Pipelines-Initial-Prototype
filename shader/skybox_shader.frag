#version 430

layout (location = 0) in vec3 Vec;

layout(binding=0) uniform samplerCube SkyBoxTex;

layout (location = 0) out vec4 FragColor;

void main()
{
	vec3 texColor = texture(SkyBoxTex, normalize(Vec)).rgb;
	FragColor = vec4(texColor, 1.0);
}