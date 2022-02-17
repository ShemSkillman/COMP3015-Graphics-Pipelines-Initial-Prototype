#version 430
in vec3 LightIntensity;
layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(LightIntensity * vec3(0.97f, 0.75f, 0.50f), 1.0);
}
