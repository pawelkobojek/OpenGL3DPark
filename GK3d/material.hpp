//
//  material.hpp
//  GK3d
//
//  Created by Paweł Kobojek on 29.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#ifndef material_hpp
#define material_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat shininess;
    
    Material() {
        
    }
    
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess)
    : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
};

#endif /* material_hpp */
