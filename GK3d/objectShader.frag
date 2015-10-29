#version 330 core

in vec3 normalVec;
in vec3 fragPos;

out vec4 color;

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define POINT_LIGHTS_COUNT 2
#define SHININESS 256

uniform PointLight pointLights[POINT_LIGHTS_COUNT];
uniform SpotLight spotLight;
uniform vec3 viewPos;
uniform vec3 objectColor;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), SHININESS);
    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance +
                                light.quadratic * (distance * distance));
    // Combine results
    vec3 ambient  = light.ambient * objectColor;  //* vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * objectColor; //* vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * objectColor; //*vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), SHININESS);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // Combine results
    vec3 ambient = light.ambient * objectColor;//vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * objectColor;//vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * objectColor;//vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main() {
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    
    vec3 norm = normalize(normalVec);
    vec3 viewDir = normalize(viewPos - fragPos);
    
//    vec3 result = calcDirLight(dirLight, norm, viewDir);
    
    for (int i = 0; i < POINT_LIGHTS_COUNT; ++i) {
        result += calcPointLight(pointLights[i], norm, fragPos, viewDir);
    }
    
    result += calcSpotLight(spotLight, norm, fragPos, viewDir); // spotlight
    
    color = vec4(result, 1.0f);
}
