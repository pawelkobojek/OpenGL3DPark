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

#define MESH_COUNT 100
#define MAX_HILL_HEIGHT 0.01f

class Model {
private:
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    GLuint VAO, VBO, EBO;
    GLfloat* modelMatrixValuePtr;
    glm::vec3 color;
public:
    Model(std::vector<glm::vec3> vertices, std::vector<GLuint> indices, glm::vec3 color);
    Model(std::vector<glm::vec3> vertices, std::vector<GLuint> indices, glm::vec3 color, GLfloat* modelMatrixValuePtr);
    ~Model();

    void setModelMatrix(GLfloat* modelMatrixValuePtr);

    void draw(GLuint shaderProgram);
    
    static Model createGround(const int meshCount = MESH_COUNT, const GLfloat maxHillHeight = MAX_HILL_HEIGHT);
//    static Model createGround(GLfloat* modelMatrixValuePtr, const int meshCount = MESH_COUNT, const GLfloat maxHillHeight = MAX_HILL_HEIGHT);
    static Model createCube();
};

#endif /* model_hpp */
