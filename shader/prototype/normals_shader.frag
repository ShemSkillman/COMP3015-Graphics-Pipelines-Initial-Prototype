#version 430

layout (location = 0) in vec2 TexCoord;
layout (location = 1) in vec3 LightDir;
layout (location = 2) in vec3 ViewDir;
layout (location = 3) in vec4 Position;

layout(binding=0) uniform sampler2D ColorTex;
layout(binding=1) uniform sampler2D NormalMapTex;

layout (location = 0) out vec4 FragColor;

uniform struct DirectionLightInfo {
 vec4 Direction;
 vec3 La;
 vec3 L;
} DirLight;

uniform struct MaterialInfo {
 vec3 Ks;
 float Shininess;
} Material;

uniform struct FogInfo
{
float MaxDist;
float MinDist;
vec3 Color;
} Fog;

vec4 blinnPhong(vec3 n)
{
	vec3 texColour = texture(ColorTex, TexCoord).xyz;

	//calculate ambient here
	vec3 ambient = texColour * DirLight.La;

	//calculate diffuse here
	float sDotN = max(dot(LightDir, n), 0.0f);
	vec3 diffuse = texColour * sDotN;

	//calculate specular here
	vec3 h = normalize(ViewDir + LightDir);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return vec4(ambient + DirLight.L * (diffuse + spec), 1.0);
}

void main()
{
	float dist = abs(Position.z);
	
	float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	// Get normal perbutation from texture
	vec3 norm = texture(NormalMapTex, TexCoord).xyz;
	norm.xy = 2.0 * norm.xy - 1.0;

	// Feed desired normal value to blinnPhong to manipulate lighting
	vec4 shadeColor = blinnPhong(normalize(norm));

	vec3 color = mix(Fog.Color, shadeColor.xyz, fogFactor);
	FragColor = vec4(color, 1.0);
}