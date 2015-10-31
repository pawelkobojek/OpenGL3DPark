//
//  light.hpp
//  GK3d
//
//  Created by Paweł Kobojek on 31.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Light {
public:
    glm::vec3 position;
    glm::vec3 color;
    
    Light(glm::vec3 position, glm::vec3 color = glm::vec3(1.0f));
    
    void attachTo(GLuint program, int index);
};

#endif /* light_hpp */
