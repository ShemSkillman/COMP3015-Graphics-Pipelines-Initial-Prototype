#version 430

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

layout (binding = 0) uniform sampler2D RenderTex;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 HdrColor;

uniform int Pass;

const vec3 lum = vec3(0.2126, 0.7152, 0.0722);

uniform float AveLum;

layout(binding=0) uniform sampler2D HdrTex;

// XYZ/RGB conversion matrices from:
// http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

uniform mat3 rgb2xyz = mat3(
 0.4124564, 0.2126729, 0.0193339,
 0.3575761, 0.7151522, 0.1191920,
 0.1804375, 0.0721750, 0.9503041 );

uniform mat3 xyz2rgb = mat3(
 3.2404542, -0.9692660, 0.0556434,
 -1.5371385, 1.8760108, -0.2040259,
 -0.4985314, 0.0415560, 1.0572252 );

uniform float Exposure = 0.35;
uniform float White = 0.928;
uniform bool DoToneMap = true;

uniform struct LightInfo {
 vec4 Position; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 L; // Diffuse and specular light intensity
} Lights[3];

uniform struct MaterialInfo {
 vec3 Ka; // Ambient reflectivity
 vec3 Kd; // Diffuse reflectivity
 vec3 Ks; // Specular reflectivity
 float Shininess; // Specular shininess factor
} Material;

vec3 blinnPhong(vec3 pos, vec3 n, int light)
{
	//calculate ambient here
	vec3 ambient = Lights[light].La * Material.Ka;

	vec3 s = normalize(Lights[light].Position.xyz - pos);
	float sDotN = max(dot(s, n), 0.0);
	vec3 diffuse = Material.Kd * sDotN;

	//calculate specular here
	vec3 v = normalize(-pos);
	vec3 h = normalize(v + s);

	vec3 spec = Material.Ks * pow(max(dot(h, n), 0.0f), Material.Shininess);
	 
	return ambient + Lights[light].L * (diffuse + spec);
}

float luminance( vec3 color )
{
 return dot(lum,color);
}


void pass1()
{
	vec3 n = normalize(Normal);
	// Compute shading and store result in high-res framebuffer
	HdrColor = vec3(0.0);
	for( int i = 0; i < 3; i++)
		HdrColor += blinnPhong(Position, n, i); 
}

void pass2()
{
	 // Retrieve high-res color from texture
	vec4 color = texture( HdrTex, TexCoord );

	 // Convert to XYZ
	 vec3 xyzCol = rgb2xyz * vec3(color);

	 // Convert to xyY
	 float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;
	 vec3 xyYCol = vec3( xyzCol.x / xyzSum, xyzCol.y / xyzSum, xyzCol.y);

	 // Apply the tone mapping operation to the luminance (xyYCol.z or xyzCol.y)
	 float L = (Exposure * xyYCol.z) / AveLum;
	 L = (L * ( 1 + L / (White * White) )) / ( 1 + L );

	 // Using the new luminance, convert back to XYZ
	 xyzCol.x = (L * xyYCol.x) / (xyYCol.y);
	 xyzCol.y = L;
	 xyzCol.z = (L * (1 - xyYCol.x - xyYCol.y))/xyYCol.y;

	 // Convert back to RGB and send to output buffer
	if( DoToneMap )
		FragColor = vec4( xyz2rgb * xyzCol, 1.0);
	else
		 FragColor = color;
}


void main()
{	
	if( Pass == 1 )
		pass1();
	else if( Pass == 2)
		pass2();
}