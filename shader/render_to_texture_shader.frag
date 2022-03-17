#version 430

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

uniform sampler2D RenderTex;

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

vec4 blinnPhong(vec3 pos, vec3 n)
{
	vec3 texColour = texture(RenderTex, TexCoord).rgb;

	//calculate ambient here
	vec3 ambient = Light.La * texColour;

	vec3 s = normalize(Light.Position.xyz - pos);
	float sDotN = max(dot(s, n), 0.0);
	vec3 diffuse = texColour * sDotN;

	//calculate specular here
	vec3 v = normalize(-pos);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return vec4(ambient + Light.L * (diffuse + spec), 1.0);
}

void main()
{
	FragColor = blinnPhong(Position, Normal);
}