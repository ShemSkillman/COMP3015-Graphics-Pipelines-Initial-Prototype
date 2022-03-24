#version 430

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;

layout (location = 0) out vec4 FragColor;

uniform struct DirectionLightInfo {
 vec4 Direction; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} DirLight;

uniform struct MaterialInfo {
 vec3 Ka; // Ambient reflectivity
 vec3 Kd; // Diffuse reflectivity
 vec3 Ks; // Specular reflectivity
 float Shininess; // Specular shininess factor
} Material;

uniform struct FogInfo
{
float MaxDist; //max distance
float MinDist; //min distance
vec3 Color; //colour of the fog
} Fog;

const int levels = 2;
const float scaleFactor = 1.0 / levels;

vec4 blinnPhong(vec4 vertexPos, vec3 n)
{
	//calculate ambient here
	vec3 ambient = Material.Ka * DirLight.La;

	vec3 s = DirLight.Direction.xyz;

	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = Material.Kd * floor(sDotN * levels) * scaleFactor;

	//calculate specular here
	vec3 v = normalize(-vertexPos.xyz);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	//return vec4(ambient + Light.L * (diffuse + spec), 1.0);
	return vec4(ambient + DirLight.L * diffuse, 1.0);
}

void main()
{
	float dist = abs(Position.z); //distance calculations

	//fogFactor calculation based on the formula presented earlier
	float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0); //we clamp values

	//colour we receive from blinnPhong calculation
	vec4 shadeColor = blinnPhong(Position, Normal);

	//we assign a colour based on the fogFactor using mix
	vec3 color = mix(Fog.Color, shadeColor.xyz, fogFactor);
	FragColor = vec4(color, 1.0); //final colour
}