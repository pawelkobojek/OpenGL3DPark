//
//  mesh.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 31.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) : vertices(vertices), indices(indices) {
    this->setupMesh();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
                 &this->vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
                 &this->indices[0], GL_STATIC_DRAW);
    
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, normal));
    
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &this->EBO);
    glDeleteBuffers(1, &this->VBO);
}

void Mesh::draw(Shader* shader) {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, (int) this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
