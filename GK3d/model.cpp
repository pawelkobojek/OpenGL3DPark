//
//  model.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 26.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include "model.hpp"

Model::Model(std::vector<glm::vec3> vertices, std::vector<GLuint> indices) : vertices(vertices), indices(indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

Model::~Model() {
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);
}

void Model::draw() {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, (int) this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

 Model Model::createGround() {
     std::vector<glm::vec3> groundVertices;
     std::vector<GLuint> groundIndices;
     const int meshCount = 100;
     
     float from = -1.0f;
     float to = 1.0f;
     float diff = to - from;
     for (int i = 0; i <= meshCount; ++i) {
         for (int j = 0; j <= meshCount; ++j) {
             float x = (i * diff / meshCount) + from;
             float z = (j * diff / meshCount) + from;
             float y = (float) rand() / (float) RAND_MAX;
             groundVertices.push_back(glm::vec3(x, y * 0.05f, z));
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
     
     return Model(groundVertices, groundIndices);
}