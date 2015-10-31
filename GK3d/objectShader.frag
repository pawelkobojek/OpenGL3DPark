#version 330 core

in vec3 normalVec;
in vec3 fragPos;

out vec4 color;

struct Light {
    vec3 position;
    vec3 color;
};

uniform vec3 objectColor;
uniform Light light;

vec3 calcDiffuse(Light light, vec3 fragPos);

void main() {
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * light.color;
    
    vec3 diffuse = calcDiffuse(light, fragPos);
    
    vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result, 1.0f);
}

vec3 calcDiffuse(Light light, vec3 fragPos) {
    vec3 norm = normalize(normalVec);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * light.color;
}