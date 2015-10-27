#version 330 core

in vec3 normalVec;
in vec3 fragPos;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewerPos;

void main() {
    
    // Ambient
    float ambientFactor = 0.1f;
    vec3 ambient = ambientFactor * lightColor;
    
    vec3 norm = normalize(normalVec);
    
    // Diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularFactor = 0.5f;
    vec3 viewDir = normalize(viewerPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularFactor * spec * lightColor;
    
    vec3 resultColor = (ambient + diffuse + specular) * objectColor;
    color = vec4(resultColor, 1.0f);
}
