//
//  light.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 31.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include "light.hpp"

Light::Light(glm::vec3 position, glm::vec3 color) : position(position), color(color) {}

void Light::attachTo(GLuint program) {
    glUniform3f(glGetUniformLocation(program, "light.position"), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(program, "light.color"), color.x, color.y, color.z);
}