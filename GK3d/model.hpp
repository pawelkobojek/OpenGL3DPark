//
//  model.hpp
//  GK3d
//
//  Created by Paweł Kobojek on 26.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#ifndef model_hpp
#define model_hpp

#include <stdio.h>
#include <vector>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model {
private:
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    GLuint VAO, VBO, EBO;
public:
    ~Model();
    Model(std::vector<glm::vec3> vertices, std::vector<GLuint> indices);
    void draw();
    
    static Model createGround();
};

#endif /* model_hpp */
