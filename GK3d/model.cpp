//
//  model.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 26.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include "model.hpp"

Model::Model(std::vector<Mesh> meshes, glm::vec3 color, Shader* shader)
: meshes(meshes), color(color), shader(shader) {

}

Model::Model(std::vector<Mesh> meshes, glm::vec3 color, Shader* shader,
             GLfloat* modelMatrixValuePtr) : Model::Model(meshes, color, shader) {
    this->modelMatrixValuePtr = modelMatrixValuePtr;
}

void Model::draw() {
    this->drawWith(this->modelMatrixValuePtr);
}

void Model::drawWith(GLfloat *modelMatrixValuePtr) {
    GLuint uniformLocation = glGetUniformLocation(shader->program, "model");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, modelMatrixValuePtr);
    glUniform3f(glGetUniformLocation(shader->program, "objectColor"), color.x, color.y, color.z);
    
    for (int i = 0; i < meshes.size(); ++i) {
        meshes[i].draw(shader);
    }
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
    
    std::vector<Mesh> meshes;
    meshes.push_back(Mesh(groundVertices, groundIndices));
    return Model(meshes, glm::vec3(0.1f, 0.8f, 0.1f), shader);
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
    
    std::vector<Mesh> meshes;
    meshes.push_back(Mesh(vertices, indices));
    return Model(meshes, glm::vec3(1.0f, 0.5f, 0.31f), shader);
}
