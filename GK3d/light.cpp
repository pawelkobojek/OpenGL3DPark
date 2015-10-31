//
//  light.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 31.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include <string>
#include <sstream>

#include "light.hpp"

Light::Light(glm::vec3 position, GLfloat constant, GLfloat linear, GLfloat quadratic, glm::vec3 color)
: position(position), color(color), constant(constant), linear(linear), quadratic(quadratic) {}

void Light::attachTo(GLuint program, int index) {
    std::ostringstream oss;
    oss << "pointLights[" << index << "].";
    std::string uniformName = oss.str();
    
    std::string posUniform = uniformName + std::string("position");
    std::string colUniform = uniformName + std::string("color");
    std::string constantUniform = uniformName + std::string("constant");
    std::string linearUniform = uniformName + std::string("linear");
    std::string quadraticUniform = uniformName + std::string("quadratic");
    
    glUniform3f(glGetUniformLocation(program, posUniform.data()), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(program, colUniform.data()), strength * color.x, strength * color.y, strength * color.z);
    glUniform1f(glGetUniformLocation(program, constantUniform.data()), constant);
    glUniform1f(glGetUniformLocation(program, linearUniform.data()), linear);
    glUniform1f(glGetUniformLocation(program, quadraticUniform.data()), quadratic);
}