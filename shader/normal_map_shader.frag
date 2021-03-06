#version 430

layout (location = 0) in vec2 TexCoord;
layout (location = 1) in vec3 LightDir;
layout (location = 2) in vec3 ViewDir;

layout(binding=0) uniform sampler2D ColorTex;
layout(binding=1) uniform sampler2D NormalMapTex;

layout (location = 0) out vec4 FragColor;

uniform struct LightInfo {
 vec4 Position; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} Light;

uniform struct MaterialInfo {
 vec3 Ks; // Specular reflectivity
 float Shininess; // Specular shininess factor
} Material;


vec4 blinnPhong(vec3 n)
{
	vec3 texColour = texture(ColorTex, TexCoord).xyz;

	//calculate ambient here
	vec3 ambient = texColour * Light.La;

	//calculate diffuse here
	float sDotN = max(dot(LightDir, n), 0.0f);
	vec3 diffuse = texColour * sDotN;

	//calculate specular here
	vec3 h = normalize(ViewDir + LightDir);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return vec4(ambient + Light.L * (diffuse + spec), 1.0);
}

void main()
{
	vec3 norm = texture(NormalMapTex, TexCoord).xyz;
	norm.xy = 2.0 * norm.xy - 1.0;

	vec4 shadeColor = blinnPhong(normalize(norm));
	FragColor = shadeColor; // final colour
}