#version 430

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

layout (binding = 0) uniform sampler2D RenderTex;

layout (location = 0) out vec4 FragColor;

uniform struct DirectionLightInfo {
 vec4 Direction; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} DirLight;

uniform struct LightInfo {
 vec4 Position; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} PointLights[2];

uniform struct MaterialInfo {
 vec3 Ka; // Ambient reflectivity
 vec3 Kd; // Diffuse reflectivity
 vec3 Ks; // Specular reflectivity
 float Shininess; // Specular shininess factor
} Material;

const int levels = 4;
const float scaleFactor = 1.0 / levels;

vec4 DirBlinnPhong(vec4 vertexPos, vec3 n)
{
	vec3 texColour = texture(RenderTex, TexCoord).rgb;

	//calculate ambient here
	vec3 ambient = texColour * DirLight.La;

	vec3 s = DirLight.Direction.xyz;

	float sDotN = max(dot(s, n), 0.0f);
	//vec3 diffuse = texColour * floor(sDotN * levels) * scaleFactor;
	vec3 diffuse = texColour * sDotN;

	//calculate specular here
	vec3 v = normalize(-vertexPos.xyz);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return vec4(ambient + DirLight.L * (diffuse + spec), 1.0);
}

vec4 PointBlinnPhong(int lightIndex, vec4 vertexPos, vec3 n)
{
	vec3 texColour = texture(RenderTex, TexCoord).rgb;

	//calculate ambient here
	vec3 ambient = texColour * PointLights[lightIndex].La;

	vec3 s = vec3(normalize(PointLights[lightIndex].Position - vertexPos));

	float sDotN = max(dot(s, n), 0.0f);
	//vec3 diffuse = texColour * floor(sDotN * levels) * scaleFactor;
	vec3 diffuse = texColour * sDotN;

	//calculate specular here
	vec3 v = normalize(-vertexPos.xyz);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return vec4(ambient + PointLights[lightIndex].L * (diffuse + spec), 1.0);
}

void main()
{
	vec4 shadeColor = DirBlinnPhong(Position, Normal);
	for (int i = 0; i < 2; i++)
	{
	 shadeColor += PointBlinnPhong(i, Position, Normal);
	}

	FragColor = shadeColor;
}