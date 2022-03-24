#version 430

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

layout (binding = 0) uniform sampler2D RenderTex;

layout (location = 0) out vec4 FragColor;

uniform float EdgeThreshold;
uniform int Pass;

const vec3 lum = vec3(0.2126, 0.7152, 0.0722);

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

const int levels = 1;
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

float luminance( vec3 color )
{
 return dot(lum,color);
}

vec4 pass1()
{
	float dist = abs(Position.z); //distance calculations

	//fogFactor calculation based on the formula presented earlier
	float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0); //we clamp values

	//colour we receive from blinnPhong calculation
	vec4 shadeColor = blinnPhong(Position, Normal);

	//we assign a colour based on the fogFactor using mix
	vec3 color = mix(Fog.Color, shadeColor.xyz, fogFactor);

	return vec4(color, 1.0);
}

vec4 pass2()
{
	 ivec2 pix = ivec2(gl_FragCoord.xy); //we grab a pixel to check if edge
	//pick neighboutring pixels for convolution filter
	//check lecture slides

	 float s00 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,1)).rgb);
	 float s10 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,0)).rgb);
	 float s20 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,-1)).rgb);
	 float s01 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(0,1)).rgb);
	 float s21 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(0,-1)).rgb);
	 float s02 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,1)).rgb);
	 float s12 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,0)).rgb);
	 float s22 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,-1)).rgb);
	 float sx = s00 + 2 * s10 + s20 - (s02 + 2 * s12 + s22);
	 float sy = s00 + 2 * s01 + s02 - (s20 + 2 * s21 + s22);
	 float g = sx * sx + sy * sy;

	 if( g > EdgeThreshold )
	 return vec4(texelFetchOffset(RenderTex, pix, 0, ivec2(1, 1)).rgb, 1.0f) * 0.8f; //edge

	 else
	 return vec4(texelFetchOffset(RenderTex, pix, 0, ivec2(0,0)).rgb, 1.0f); //no edge
	 //return vec4(0.0,0.0,0.0,1.0); //no edge
}

void main()
{
	if (Pass == 1) FragColor = pass1();
	if (Pass == 2) FragColor = pass2();
}