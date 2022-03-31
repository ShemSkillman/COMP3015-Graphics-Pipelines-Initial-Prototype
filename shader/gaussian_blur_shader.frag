#version 430

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

layout (binding = 0) uniform sampler2D RenderTex;

layout (location = 0) out vec4 FragColor;

uniform int Pass;
uniform float Weight[5];

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

vec4 blinnPhong(vec3 pos, vec3 n)
{
	//calculate ambient here
	vec3 ambient = Light.La * Material.Ka;

	vec3 s = normalize(Light.Position.xyz - pos);
	float sDotN = max(dot(s, n), 0.0);
	vec3 diffuse = Material.Kd * sDotN;

	//calculate specular here
	vec3 v = normalize(-pos);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return vec4(ambient + Light.L * (diffuse + spec), 1.0);
}

float luminance( vec3 color )
{
 return dot(lum,color);
}


vec4 pass1()
{
	return blinnPhong(Position, Normal);
}

vec4 pass2()
{
	ivec2 pix = ivec2( gl_FragCoord.xy );
	vec4 sum = texelFetch(RenderTex, pix, 0) * Weight[0];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(0,1) ) * Weight[1];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(0,-1) ) * Weight[1];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(0,2) ) * Weight[2];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(0,-2) ) * Weight[2];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(0,3) ) * Weight[3];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(0,-3) ) * Weight[3];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(0,4) ) * Weight[4];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(0,-4) ) * Weight[4];
	return sum;
}

vec4 pass3()
{
	ivec2 pix = ivec2( gl_FragCoord.xy );
	vec4 sum = texelFetch(RenderTex, pix, 0) * Weight[0];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(1,0) ) * Weight[1];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(-1,0) ) * Weight[1];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(2,0) ) * Weight[2];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(-2,0) ) * Weight[2];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(3,0) ) * Weight[3];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(-3,0) ) * Weight[3];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(4,0) ) * Weight[4];
	sum += texelFetchOffset( RenderTex, pix, 0, ivec2(-4,0) ) * Weight[4];
	return sum;

}


void main()
{	
	if (Pass == 1) FragColor = pass1();
	if (Pass == 2) FragColor = pass2();
	if (Pass == 3) FragColor = pass3();
}