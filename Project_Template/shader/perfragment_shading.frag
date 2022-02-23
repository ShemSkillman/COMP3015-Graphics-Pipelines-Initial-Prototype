#version 430

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

out vec3 FragColor;

uniform struct LightInfo {
 vec4 Position; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} Light;

uniform struct SpotLightInfo {
vec3 Position; // Position in cam coords
vec3 L; // Diffuse/spec intensity
vec3 La; // Amb intensity
vec3 Direction; // Direction of the spotlight in cam coords.
float Exponent; // Angular attenuation exponent
float Cutoff; // Cutoff angle (between 0 and pi/2)
} Spot;

uniform struct FogInfo
{
float MaxDist; //max distance
float MinDist; //min distance
vec3 Color; //colour of the fog
} Fog;

uniform struct MaterialInfo {
 vec3 Ka; // Ambient reflectivity
 vec3 Kd; // Diffuse reflectivity
 vec3 Ks; // Specular reflectivity
 float Shininess; // Specular shininess factor
} Material;


vec3 phongModel(vec4 vertexPos, vec3 n)
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
	 
	return ambient + Light.L * (diffuse + spec);
}

vec3 blinnPhongSpot(vec3 position, vec3 n)
{
	vec3 ambient = Material.Ka * Spot.La;
	vec3 s = vec3(normalize(Spot.Position - position));

	float cosAng = dot(-s, normalize(Spot.Direction)); //cosine of the angle
	float angle = acos(cosAng); //gives you the actual angle

	float spotScale = 0.0;

	if(angle < Spot.Cutoff)
	{
		spotScale = pow(cosAng, Spot.Exponent);
	}

	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = Material.Kd * sDotN;

	vec3 v = normalize(-position.xyz);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);

	return ambient + spotScale * Spot.L * (diffuse + spec);
}

void main()
{
	FragColor = blinnPhongSpot(Position, Normal);
}