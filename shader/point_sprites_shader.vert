#version 430

layout (location = 0) in vec3 VertexPosition;

uniform mat4 ModelViewMatrix;

void main() { 
	gl_Position = ModelViewMatrix * vec4(VertexPosition, 1.0);
}