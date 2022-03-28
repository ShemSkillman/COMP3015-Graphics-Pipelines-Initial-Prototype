#version 430

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

layout (binding = 0) uniform sampler2D RenderTex;

layout (location = 0) out vec4 FragColor;

uniform struct DirectionLightInfo {
 vec4 Direction;
 vec3 La;
 vec3 L;
} DirLight;

uniform struct SpotLightInfo {
vec4 Position;
vec3 L;
vec3 La;
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

vec4 dirBlinnPhong(vec4 vertexPos, vec3 n)
{
	vec3 texColour = texture(RenderTex, TexCoord).rgb;

	//calculate ambient here
	vec3 ambient = texColour * DirLight.La;

	vec3 s = DirLight.Direction.xyz;

	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = texColour * sDotN;

	vec4 eyeCoords = -vertexPos;

	//calculate specular here

	// direction from vertex pos to eye
	vec3 v = normalize(eyeCoords.xyz);

	// approximate reflection vector (optimization)
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return vec4(ambient + DirLight.L * (diffuse + spec), 1.0);
}

vec4 blinnPhongSpot(vec4 vertexPos, vec3 n)
{
	vec3 texColour = texture(RenderTex, TexCoord).rgb;

	vec3 ambient = texColour * Spot.La; //calculate ambient	

	// Direction from vertex position to spot light position
	vec3 s = vec3(normalize(Spot.Position - vertexPos));

	// Use direction from spot light position to vertex position
	float cosAng = dot(-s, normalize(Spot.Direction)); //cosine of the angle
	
	// calculate angle between spotlight center and vertex position
	float angle = acos(cosAng);

	float spotScale = 0.0;

	// Does the spotlight angle cover this area?
	// If not, the area is left completely black (with the exception of ambience)
	if(angle < Spot.Cutoff )
	{
		spotScale = pow( cosAng, Spot.Exponent );
	}

	float sDotN = max(dot(s, n), 0.0f);

	vec3 diffuse = texColour * sDotN;

	vec4 eyeCoords = -vertexPos;
		
	vec3 v = normalize(eyeCoords.xyz);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);

	return vec4(ambient + spotScale * Spot.L * (diffuse + spec), 1.0);
}

void main()
{
	float dist = abs(Position.z); //distance calculations

	//fogFactor calculation based on the formula presented earlier
	float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0); //we clamp values

	vec4 shadeColor = dirBlinnPhong(Position, Normal) + blinnPhongSpot(Position, Normal);

	vec3 color = mix(Fog.Color, shadeColor.xyz, fogFactor);

	FragColor = vec4(color, 1.0);
}