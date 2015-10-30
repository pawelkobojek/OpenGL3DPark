//
//  model.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 26.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include "model.hpp"

Model::Model(std::vector<glm::vec3> vertices, std::vector<GLuint> indices, Material material, Shader* shader)
: vertices(vertices), indices(indices), material(material), shader(shader) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
}

Model::Model(std::vector<glm::vec3> vertices, std::vector<GLuint> indices, Material material, Shader* shader,
             GLfloat* modelMatrixValuePtr) : Model::Model(vertices, indices, material, shader) {
    this->modelMatrixValuePtr = modelMatrixValuePtr;
}

Model::~Model() {
//    glDeleteBuffers(1, &EBO);
//    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);
}

void Model::draw() {
    this->drawWith(this->modelMatrixValuePtr);
}

void Model::drawWith(GLfloat *modelMatrixValuePtr) {
    GLuint uniformLocation = glGetUniformLocation(shader->program, "model");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, modelMatrixValuePtr);
    glUniform3f(glGetUniformLocation(shader->program, "material.ambient"), material.ambient.x, material.ambient.y,
                material.ambient.z);
    glUniform3f(glGetUniformLocation(shader->program, "material.diffuse"), material.diffuse.x, material.diffuse.y,
                material.diffuse.z);//1.0f, 0.5f, 0.31f);
    glUniform3f(glGetUniformLocation(shader->program, "material.specular"),  material.specular.x, material.specular.y,
                material.specular.z);//0.5f, 0.5f, 0.5f);
    glUniform1f(glGetUniformLocation(shader->program, "material.shininess"), material.shininess);
    
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, (int) this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model::setModelMatrix(GLfloat* modelMatrixValuePtr) {
    this->modelMatrixValuePtr = modelMatrixValuePtr;
}

void Model::setLightColor(glm::vec3 lightColor) {
    this->lightColor = lightColor;
}

Model Model::createGround(Shader* shader, const int meshCount, const GLfloat maxHillHeight) {
    std::vector<glm::vec3> groundVertices;
    std::vector<GLuint> groundIndices;
    
    float from = -1.0f;
    float to = 1.0f;
    float diff = to - from;
    for (int i = 0; i <= meshCount; ++i) {
        for (int j = 0; j <= meshCount; ++j) {
            float x = (i * diff / meshCount) + from;
            float z = (j * diff / meshCount) + from;
            float y = (float) rand() / (float) RAND_MAX;
            groundVertices.push_back(glm::vec3(x, y * maxHillHeight, z));
        }
    }
    
    for (int i = 0; i < meshCount; ++i) {
        for (int j = 0; j < meshCount; ++j) {
            groundIndices.push_back((i) * (meshCount + 1) + j);
            groundIndices.push_back((i + 1) * (meshCount + 1) + j);
            groundIndices.push_back((i) * (meshCount + 1) + j + 1);
            
            groundIndices.push_back((i) * (meshCount + 1) + j + 1);
            groundIndices.push_back((i + 1) * (meshCount + 1) + j);
            groundIndices.push_back((i + 1) * (meshCount + 1) + j + 1);
        }
    }
    
    Material material = Material(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f),
                                 glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f * 128.0f);
    return Model(groundVertices, groundIndices, material, shader);
}

Model Model::createCube(Shader* shader) {
    GLfloat cubeVerts[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;

    int j = 0;
    for (int i = 0; i < 36*6; i+=6) {
        glm::vec3 vert(cubeVerts[i], cubeVerts[i+1], cubeVerts[i+2]);
        glm::vec3 normal(cubeVerts[i+3], cubeVerts[i+4], cubeVerts[i+5]);
        vertices.push_back(vert);
        vertices.push_back(normal);
        indices.push_back(j++);
        indices.push_back(j++);
        indices.push_back(j++);
    }
    
    Material material = Material(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f),
                                 glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f * 128.0f);
    return Model(vertices, indices, material, shader);
}
