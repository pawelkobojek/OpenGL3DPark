#version 330 core

in vec3 normalVec;
in vec3 fragPos;

out vec4 color;

struct Light {
    vec3 position;
    vec3 color;
};

uniform vec3 viewerPos;
uniform vec3 objectColor;
uniform Light light;

vec3 calcAmbient(Light light);
vec3 calcDiffuse(Light light, vec3 lightDir, vec3 norm);
vec3 calcSpecular(Light light, vec3 lightDir, vec3 viewerPos, vec3 fragPos, vec3 norm);

void main() {
    vec3 norm = normalize(normalVec);
    vec3 lightDir = normalize(light.position - fragPos);
    
    vec3 ambient = calcAmbient(light);
    vec3 diffuse = calcDiffuse(light, lightDir, norm);
    vec3 specular = calcSpecular(light, lightDir, viewerPos, fragPos, norm);
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
}

vec3 calcAmbient(Light light) {
    float ambientStrength = 0.1f;
    return ambientStrength * light.color;
}

vec3 calcDiffuse(Light light, vec3 lightDir, vec3 norm) {
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * light.color;
}

vec3 calcSpecular(Light light, vec3 lightDir, vec3 viewerPos, vec3 fragPos, vec3 norm) {
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    return specularStrength * spec * light.color;
}