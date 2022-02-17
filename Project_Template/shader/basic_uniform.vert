#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform vec3 Ka;
uniform vec3 La;
uniform vec4 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform int SpecularPowerCoefficient;
uniform vec3 ViewPosition;
uniform vec3 Ks;
uniform vec3 Ls;

out vec3 Colour;

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

void getNormAndPos(out vec3 n, out vec4 vertexPos)
{
	n = normalize(NormalMatrix * VertexNormal);
	vertexPos = ModelViewMatrix * vec4(VertexPosition, 1.0);
}

vec3 phongModel(int light, vec3 vertexPos, vec3 n)
{
	//calculate ambient here
	vec3 ambient = Material.Ka * lights[light].La;

	//calculate diffuse here
	vec3 s = vec3(normalize(lights[light].Position - vertexPos))
	float sDotN = max(dot(s, n), 0.0f)
	vec3 diffuse = Material.Kd * sDotN;

	//calculate specular here
	vec3 r = reflect(-s, n);
	vec3 spec = Material.Ks * pow(max(dot(r, ViewPosition), 0.0f), Material.Shininess);
	 
	return ambient + lights[light].L * (diffuse + spec);
}

void main()
{
	vec3 n;
	vec4 vertexPos;
	getNormAndPos(n, vertexPos);

	Colour = vec3(0.0);
	for(int i = 0; i < 3; i++)
		Colour += phongModel(i, vertexPos, n);

	Colour *= vec3(0.97f, 0.75f, 0.50f);

    gl_Position =  MVP * vec4(VertexPosition,1.0);
}