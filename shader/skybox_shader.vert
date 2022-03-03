#version 430

layout (location = 0) in vec3 VertexPosition;

layout (location = 0) out vec4 Position;

uniform mat4 MVP;

void main() {
	Position = MVP * vec4(VertexPosition, 1.0f);

	gl_Position = Position;
}