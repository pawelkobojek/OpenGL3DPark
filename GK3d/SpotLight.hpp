//
//  SpotLight.hpp
//  GK3d
//
//  Created by Paweł Kobojek on 31.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#ifndef SpotLight_hpp
#define SpotLight_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "light.hpp"

class SpotLight : Light {
public:
    glm::vec3 direction;
    
    GLfloat cutOff;
    GLfloat outerCutOff;
    
    SpotLight(glm::vec3 position, glm::vec3 direction, GLfloat cutOff, GLfloat outerCutOff, GLfloat constant,
              GLfloat linear, GLfloat quadratic, glm::vec3 color = glm::vec3(1.0f));
    
    void attachTo(GLuint program, int index);
    
    void setPosition(glm::vec3 pos);
    void setStrength(GLfloat strength);
};

#endif /* SpotLight_hpp */
