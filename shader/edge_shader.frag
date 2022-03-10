#version 430

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

layout(binding=0) uniform sampler RenderTex;

layout (location = 0) out vec4 FragColor;

uniform float EdgeThreshold;
uniform int Pass;

const vec3 lum = vec3(0.2126, 0.7152, 0.0722);

uniform struct LightInfo {
 vec4 Position; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} Light;

uniform struct MaterialInfo {
 vec3 Ka; // Ambient reflectivity
 vec3 Kd; // Diffuse reflectivity
 vec3 Ks; // Specular reflectivity
 float Shininess; // Specular shininess factor
} Material;

vec4 blinnPhong(vec4 vertexPos, vec3 n)
{
	//calculate ambient here
	vec3 ambient = Material.Ka * Light.La;

	//calculate diffuse here
	vec3 s = vec3(normalize(Light.Position - vertexPos));
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = Material.Kd * sDotN;

	//calculate specular here
	vec3 v = normalize(-vertexPos.xyz);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return vec4(ambient + Light.L * (diffuse + spec), 1.0);
}

void main()
{
	
	//FragColor = vec4(texColor, 1.0);
}