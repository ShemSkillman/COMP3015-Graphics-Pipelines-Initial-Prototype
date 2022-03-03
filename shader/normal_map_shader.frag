#version 430

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in vec3 LightDir;
layout (location = 4) in vec3 ViewDir;

layout(binding=0) uniform sampler2D ColorTex;
layout(binding=1) uniform sampler2D NormalMapTex;

layout (location = 0) out vec4 FragColor;

uniform struct LightInfo {
 vec4 Position; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} lights[3];

uniform struct MaterialInfo {
 vec3 Ka; // Ambient reflectivity
 vec3 Kd; // Diffuse reflectivity
 vec3 Ks; // Specular reflectivity
 float Shininess; // Specular shininess factor
} Material;

uniform bool Discard;

vec4 blinnPhong(int light, vec4 vertexPos, vec3 n)
{
	vec3 texColour = texture(ColorTex, TexCoord).xyz;

	//calculate ambient here
	vec3 ambient = texColour * lights[light].La;

	//calculate diffuse here
	float sDotN = max(dot(LightDir, n), 0.0f);
	vec3 diffuse = texColour * sDotN;

	//calculate specular here
	vec3 h = normalize(ViewDir + LightDir);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return vec4(ambient + lights[light].L * (diffuse + spec), 1.0);
}

void main()
{
	vec3 norm = texture(NormalMapTex, TexCoord).xyz;
	norm.xy = 2.0 * norm.xy - 1.0;

	vec4 shadeColor = vec4(0.0f);
	for (int i = 0; i < 3; i++)
	{
	 shadeColor += blinnPhong(i, Position, normalize(norm));
	}

	FragColor = shadeColor; // final colour
}