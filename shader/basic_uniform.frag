#version 430

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;

layout (location = 0) out vec4 FragColor;

uniform struct LightInfo {
 vec4 Position; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} lights[3];

uniform struct SpotLightInfo {
vec4 Position; // Position in cam coords
vec3 L; // Diffuse/spec intensity
vec3 La; // Amb intensity
vec3 Direction; // Direction of the spotlight in cam coords.
float Exponent; // Angular attenuation exponent
float Cutoff; // Cutoff angle (between 0 and pi/2)
} Spot;

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

vec4 blinnPhong(int light, vec4 vertexPos, vec3 n)
{
	//calculate ambient here
	vec3 ambient = Material.Ka * lights[light].La;

	//calculate diffuse here
	vec3 s = vec3(normalize(lights[light].Position - vertexPos));
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = Material.Kd * sDotN;

	//calculate specular here
	vec3 v = normalize(-vertexPos.xyz);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return vec4(ambient + lights[light].L * (diffuse + spec), 1.0);
}

vec3 blinnPhongSpot(vec4 vertexPos, vec3 n)
{
	vec3 ambient = Material.Ka * Spot.La; //calculate ambient
	vec3 s = vec3(normalize(Spot.Position - vertexPos));

	float cosAng = dot(-s, normalize(Spot.Direction)); //cosine of the angle
	float angle = acos(cosAng); //gives you the actual angle

	float spotScale = 0.0;
	if(angle < Spot.Cutoff )
	{
		spotScale = pow( cosAng, Spot.Exponent );
	}

	float sDotN = max(dot(s, n), 0.0f);

	vec3 diffuse = Material.Kd * sDotN;
		
	vec3 v = normalize(-vertexPos.xyz);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);

	return ambient + spotScale * Spot.L * (diffuse + spec);
}

void main()
{
	float dist = abs(Position.z); //distance calculations

	//fogFactor calculation based on the formula presented earlier
	float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0); //we clamp values

	//colour we receive from blinnPhong calculation
	vec4 shadeColor = vec4(0.0f);
	for (int i = 0; i < 3; i++)
	{
	 shadeColor += blinnPhong(i, Position, normalize(Normal));
	}

	//we assign a colour based on the fogFactor using mix
	vec3 color = mix(Fog.Color, shadeColor.xyz, fogFactor);
	FragColor = vec4(color, 1.0); //final colour
}