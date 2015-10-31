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

#include "shader.hpp"
#include "material.hpp"
#include "vertex.hpp"

#define MESH_COUNT 100
#define MAX_HILL_HEIGHT 0.0f

class Model {
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint VAO, VBO, EBO;
    GLfloat* modelMatrixValuePtr;
public:
    Shader* shader;
    Material material;
    
    Model(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material material, Shader* shader);
    Model(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material material, Shader* shader,
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
