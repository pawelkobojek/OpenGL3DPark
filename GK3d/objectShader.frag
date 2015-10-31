#version 330 core

struct PointLight {
    vec3 position;
    vec3 color;
    
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
};

#define POINT_LIGHTS_COUNT 2

uniform vec3 viewerPos;
uniform vec3 objectColor;
uniform SpotLight spotLight;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];

in vec3 normalVec;
in vec3 fragPos;

out vec4 color;

float calcAttenuation(vec3 lightPosition, float constant, float linear, float quadratic, vec3 fragPos);

vec3 calcAmbient(vec3 lightColor, float strentgh);
vec3 calcDiffuse(vec3 lightColor, vec3 lightDir, vec3 norm);
vec3 calcSpecular(vec3 lightColor, vec3 lightDir, vec3 viewerPos, vec3 fragPos, vec3 norm);

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 viewerPos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 viewerPos);

void main() {
    vec3 norm = normalize(normalVec);
    vec3 result = vec3(0.0f);
    vec3 viewDir = normalize(viewerPos - fragPos);
    
    for (int i = 0; i < POINT_LIGHTS_COUNT; ++i) {
        result += calcPointLight(pointLights[i], norm, fragPos, viewDir, viewerPos);
    }
    
    result += calcSpotLight(spotLight, norm, fragPos, viewDir, viewerPos);
    
    color = vec4(result, 1.0f);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 viewerPos) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 ambient = calcAmbient(light.color, 0.1f);
    vec3 diffuse = calcDiffuse(light.color, lightDir, normal);
    vec3 specular = calcSpecular(light.color, lightDir, viewerPos, fragPos, normal);
    float attenuation = calcAttenuation(light.position, light.constant, light.linear, light.quadratic, fragPos);
    
    return attenuation * (ambient + diffuse + specular) * objectColor;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 viewerPos) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    float theta = dot(light.direction, normalize(-lightDir));
    float eps = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / eps, 0.0f, 1.0f);
    
    vec3 ambient = calcAmbient(light.color, 0.0f);
    vec3 diffuse = calcDiffuse(light.color, lightDir, normal);
    vec3 specular = calcSpecular(light.color, lightDir, viewerPos, fragPos, normal);
    
    float attenuation = calcAttenuation(light.position, light.constant, light.linear, light.quadratic, fragPos);
    
    return light.color * ((specular + diffuse) * intensity * attenuation) * objectColor; // * objectColor or not?
}

vec3 calcAmbient(vec3 lightColor, float strentgh) {
    return strentgh * lightColor;
}

vec3 calcDiffuse(vec3 lightColor, vec3 lightDir, vec3 norm) {
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * lightColor;
}

vec3 calcSpecular(vec3 lightColor, vec3 lightDir, vec3 viewerPos, vec3 fragPos, vec3 norm) {
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    return specularStrength * spec * lightColor;
}

float calcAttenuation(vec3 lightPosition, float constant, float linear, float quadratic, vec3 fragPos) {
    float dist = length(lightPosition - fragPos);
    float f = constant + (dist * linear) + (dist * dist * quadratic);
    
    return 1.0f / f;
}

