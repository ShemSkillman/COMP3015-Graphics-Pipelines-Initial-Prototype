#version 430

const float PI = 3.14159265359;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexAge;

// Render pass
uniform int Pass;

// Output to transform feedback buffers Update Pass
out vec3 Position;
out vec3 Velocity;
out float Age;

// Out to fragment shader
out float Transp;
out vec2 TexCoord;

uniform float Time;
uniform float DeltaT;
uniform vec3 Accel;
uniform float ParticleLifeTime;
uniform vec3 Emitter = vec3(0);
uniform mat3 EmitterBasis;
uniform float ParticleSize;

uniform mat4 MV;
uniform mat4 Proj;

uniform sampler1D RandomTex;

const vec3 offsets[] = vec3[](vec3(-0.5, -0.5, 0), vec3(0.5, -0.5, 0), vec3(0.5, 0.5, 0),
							  vec3(-0.5, -0.5, 0), vec3(0.5, 0.5, 0), vec3(-0.5, 0.5, 0));

// Texture coords for each vertex of particl's quads
const vec2 texCoords[] = vec2[](vec2(0,0), vec2(1,0), vec2(1,1), vec2(0,0), vec2(1,1), vec2(0, 1));

vec3 randomInitialVelocity() {
	float theta = mix(0.0, PI / 8.0, texelFetch(RandomTex, 3 * gl_VertexID, 0).r);
	float phi = mix(0.0, 2.0 * PI, texelFetch(RandomTex, 3 * gl_VertexID + 1, 0).r);
	float velocity = mix(1.25, 1.5, texelFetch(RandomTex, 3 * gl_VertexID + 2, 0).r);
	vec3 v = vec3(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
	return normalize(EmitterBasis * v) * velocity;
}

void update() {
	if (VertexAge < 0 || VertexAge > ParticleLifeTime) {
	// The particle is past it's lifetime, recycle.
	Position = Emitter;
	Velocity = randomInitialVelocity();
	if (VertexAge < 0) Age = VertexAge + DeltaT;
	else Age = (VertexAge - ParticleLifeTime) + DeltaT;
	} 
	else {
		// The particle is alive, update.
		Position = VertexPosition + VertexVelocity * DeltaT;
		Velocity = VertexVelocity + Accel * DeltaT;
		Age = VertexAge + DeltaT;
	}
}

void render() {
	Transp = 0.0;
	vec3 posCam = vec3(0.0);
	if (VertexAge >= 0.0) {
		posCam = (MV * vec4(VertexPosition, 1)).xyz + offsets[gl_VertexID] * ParticleSize;
		Transp = clamp(1.0 - VertexAge / ParticleLifeTime, 0, 1);
	}
	TexCoord = texCoords[gl_VertexID];

	gl_Position = Proj * vec4(posCam, 1);
}

void main()
{
	if (Pass == 1)
		update();
	else
		render();
}