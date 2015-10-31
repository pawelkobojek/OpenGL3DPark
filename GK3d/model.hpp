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
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "material.hpp"
#include "vertex.hpp"

#define MESH_COUNT 100
#define MAX_HILL_HEIGHT 0.005f

class Model {
private:
    GLuint VAO, VBO, EBO;
    GLfloat* modelMatrixValuePtr;
    glm::vec3 color;
public:
    Shader* shader;
    Material material;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    
    Model(std::vector<Vertex> vertices, std::vector<GLuint> indices, glm::vec3 color, Shader* shader);
    Model(std::vector<Vertex> vertices, std::vector<GLuint> indices, glm::vec3 color, Shader* shader,
          GLfloat* modelMatrixValuePtr);
    ~Model();

    void setModelMatrix(GLfloat* modelMatrixValuePtr);
    void setLightColor(glm::vec3 lightColor);

    void draw();
    void drawWith(GLfloat* modelMatrixValuePtr);
    
    static Model createGround(Shader* shader, const int meshCount = MESH_COUNT,
                              const GLfloat maxHillHeight = MAX_HILL_HEIGHT);
    static Model createCube(Shader* shader);
};

#endif /* model_hpp */
