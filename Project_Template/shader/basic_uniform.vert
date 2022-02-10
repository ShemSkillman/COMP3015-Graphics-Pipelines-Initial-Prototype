#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec3 Color;

uniform mat4 RotationMatrix;

uniform vec4 LightPosition;
uniform vec4 Kd;
uniform vec4 Ld;

void main()
{
    Color = VertexColor;
    gl_Position = vec4(VertexPosition,1.0) * RotationMatrix;
}
