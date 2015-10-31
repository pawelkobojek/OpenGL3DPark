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

Light::Light(glm::vec3 position, glm::vec3 color) : position(position), color(color) {}

void Light::attachTo(GLuint program, int index) {
    std::ostringstream oss;
    oss << "pointLights[" << index << "].";
    std::string uniformName = oss.str();
    
    std::string pos = uniformName + std::string("position");
    std::string col = uniformName + std::string("color");
    
    glUniform3f(glGetUniformLocation(program, pos.data()), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(program, col.data()), color.x, color.y, color.z);
}