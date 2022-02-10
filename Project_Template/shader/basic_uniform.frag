#version 430
in vec3 LightIntensity;
layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(LightIntensity * vec3(0.26f, 0.53f, 0.96f), 1.0);
}
