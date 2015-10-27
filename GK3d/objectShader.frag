#version 330 core

in vec3 normalVec;
in vec3 fragPos;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
    
    // Ambient
    float ambientFactor = 0.4f;
    vec3 ambient = ambientFactor * lightColor;
    
    // Diffuse
    vec3 norm = normalize(normalVec);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 resultColor = (ambient + diffuse) * objectColor;
    color = vec4(resultColor, 1.0f);
}
