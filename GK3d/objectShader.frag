#version 330 core

struct PointLight {
    vec3 position;
    vec3 color;
    
    float constant;
    float linear;
    float quadratic;
};

#define POINT_LIGHTS_COUNT 2

uniform vec3 viewerPos;
uniform vec3 objectColor;
uniform PointLight light;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];

in vec3 normalVec;
in vec3 fragPos;

out vec4 color;

float calcAttenuation(PointLight light, vec3 fragPos);

vec3 calcAmbient(PointLight light);
vec3 calcDiffuse(PointLight light, vec3 lightDir, vec3 norm);
vec3 calcSpecular(PointLight light, vec3 lightDir, vec3 viewerPos, vec3 fragPos, vec3 norm);

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos);

void main() {
    vec3 norm = normalize(normalVec);
    vec3 result = vec3(0.0f);
    
    for (int i = 0; i < POINT_LIGHTS_COUNT; ++i) {
        result += calcPointLight(pointLights[i], norm, fragPos);
    }
    
    color = vec4(result, 1.0f);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 ambient = calcAmbient(light);
    vec3 diffuse = calcDiffuse(light, lightDir, normal);
    vec3 specular = calcSpecular(light, lightDir, viewerPos, fragPos, normal);
    float attenuation = calcAttenuation(light, fragPos);
    
    return attenuation * (ambient + diffuse + specular) * objectColor;
}

vec3 calcAmbient(PointLight light) {
    float ambientStrength = 0.1f;
    return ambientStrength * light.color;
}

vec3 calcDiffuse(PointLight light, vec3 lightDir, vec3 norm) {
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * light.color;
}

vec3 calcSpecular(PointLight light, vec3 lightDir, vec3 viewerPos, vec3 fragPos, vec3 norm) {
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    return specularStrength * spec * light.color;
}

float calcAttenuation(PointLight light, vec3 fragPos) {
    float dist = length(light.position - fragPos);
    float f = light.constant + (dist * light.linear) + (dist * dist * light.quadratic);
    
    return 1.0f / f;
}

