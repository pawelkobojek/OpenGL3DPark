#version 330 core

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    color = vec4(lightColor, 1.0f);
    color = vec4(objectColor, 1.0f);
    color = vec4(1.0f);
}