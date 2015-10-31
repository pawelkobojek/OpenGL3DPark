//
//  SpotLight.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 31.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include <string>
#include "SpotLight.hpp"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, GLfloat cutOff, GLfloat outerCutOff, GLfloat constant,
                     GLfloat linear, GLfloat quadratic, glm::vec3 color)
: Light(position, constant, linear, quadratic, color), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff) {
    
}

void SpotLight::attachTo(GLuint program, int index) {
    std::string uniformName = std::string("spotLight.");
    
    std::string posUniform =         uniformName + std::string("position");
    std::string colUniform =         uniformName + std::string("color");
    std::string constantUniform =    uniformName + std::string("constant");
    std::string linearUniform =      uniformName + std::string("linear");
    std::string quadraticUniform =   uniformName + std::string("quadratic");
    std::string directionUniform =   uniformName + std::string("direction");
    std::string cutOffUniform =      uniformName + std::string("cutOff");
    std::string outerCutOffUniform = uniformName + std::string("outerCutOff");
    
    glUniform3f(glGetUniformLocation(program, posUniform.data()), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(program, colUniform.data()), strength * color.x, strength * color.y, strength * color.z);
    glUniform1f(glGetUniformLocation(program, constantUniform.data()), constant);
    glUniform1f(glGetUniformLocation(program, linearUniform.data()), linear);
    glUniform1f(glGetUniformLocation(program, quadraticUniform.data()), quadratic);
    glUniform3f(glGetUniformLocation(program, directionUniform.data()), direction.x, direction.y, direction.z);
    glUniform1f(glGetUniformLocation(program, cutOffUniform.data()), cutOff);
    glUniform1f(glGetUniformLocation(program, outerCutOffUniform.data()), outerCutOff);
}

void SpotLight::setPosition(glm::vec3 pos) {
    this->position = pos;
}

void SpotLight::setStrength(GLfloat strength) {
    this->strength = strength;
}