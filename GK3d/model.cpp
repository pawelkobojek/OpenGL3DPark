//
//  model.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 26.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include "model.hpp"

Model::Model(std::vector<Vertex> vertices, std::vector<GLuint> indices, glm::vec3 color, Shader* shader)
: vertices(vertices), indices(indices), color(color), shader(shader) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
    
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
}

Model::Model(std::vector<Vertex> vertices, std::vector<GLuint> indices, glm::vec3 color, Shader* shader,
             GLfloat* modelMatrixValuePtr) : Model::Model(vertices, indices, color, shader) {
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
    glUniform3f(glGetUniformLocation(shader->program, "objectColor"), color.x, color.y, color.z);
    
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, (int) this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model::setModelMatrix(GLfloat* modelMatrixValuePtr) {
    this->modelMatrixValuePtr = modelMatrixValuePtr;
}

Model Model::createGround(Shader* shader, const int meshCount, const GLfloat maxHillHeight) {
    std::vector<Vertex> groundVertices;
    std::vector<GLuint> groundIndices;
    
    float from = -1.0f;
    float to = 1.0f;
    float diff = to - from;
    for (int i = 0; i <= meshCount; ++i) {
        for (int j = 0; j <= meshCount; ++j) {
            float x = (i * diff / meshCount) + from;
            float z = (j * diff / meshCount) + from;
            float y = (float) rand() / (float) RAND_MAX;
            
            Vertex v;
            v.position = glm::vec3(x, y * maxHillHeight, z);
            groundVertices.push_back(v);
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
    
    for (int i = 0; i < groundIndices.size(); i += 3) {
        glm::vec3 v[] = { groundVertices[groundIndices[i]].position, groundVertices[groundIndices[i+1]].position,
                           groundVertices[groundIndices[i+2]].position };
        glm::vec3 norm = glm::cross(v[2] - v[0], v[1] - v[0]);
        
        groundVertices[groundIndices[i]].normal += norm;
        groundVertices[groundIndices[i+1]].normal += norm;
        groundVertices[groundIndices[i+2]].normal += norm;
        
    }
    
    for (int i = 0; i < groundVertices.size(); ++i) {
        groundVertices[i].normal = glm::normalize(groundVertices[i].normal);
    }
    
    return Model(groundVertices, groundIndices, glm::vec3(0.1f, 0.8f, 0.1f), shader);
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
    
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    int j = 0;
    for (int i = 0; i < 36*6; i+=6) {
        Vertex v;
        v.position = glm::vec3(cubeVerts[i], cubeVerts[i+1], cubeVerts[i+2]);
        v.normal = glm::vec3(cubeVerts[i+3], cubeVerts[i+4], cubeVerts[i+5]);
        vertices.push_back(v);
        indices.push_back(j++);
        indices.push_back(j++);
        indices.push_back(j++);
    }
    
    return Model(vertices, indices, glm::vec3(1.0f, 0.5f, 0.31f), shader);
}
