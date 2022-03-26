#version 430

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

layout (binding = 0) uniform sampler2D RenderTex;

layout (location = 0) out vec4 FragColor;

uniform struct DirectionLightInfo {
 vec4 Direction;
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

void main()
{
	float dist = abs(Position.z); //distance calculations

	//fogFactor calculation based on the formula presented earlier
	float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0); //we clamp values

	vec4 shadeColor = DirBlinnPhong(Position, Normal);

	vec3 color = mix(Fog.Color, shadeColor.xyz, fogFactor);

	FragColor = vec4(color, 1.0);
}