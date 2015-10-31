//
//  mesh.hpp
//  GK3d
//
//  Created by Paweł Kobojek on 31.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#ifndef mesh_hpp
#define mesh_hpp

#include <stdio.h>
#include <vector>
#include <GL/glew.h>

#include "vertex.hpp"
#include "shader.hpp"

class Mesh {
private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    // TODO - textures?
    
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    void draw(Shader* shader);
};

#endif /* mesh_hpp */
