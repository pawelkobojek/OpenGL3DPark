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
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.hpp"
#include "mesh.hpp"
#include "vertex.hpp"

#define MESH_COUNT 100
#define MAX_HILL_HEIGHT 0.005f

class Model {
private:
    GLfloat* modelMatrixValuePtr;
    glm::vec3 color;
    
    std::string directory;
    
    Model();
public:
    Shader* shader;
    std::vector<Mesh> meshes;
    
    Model(std::vector<Mesh> meshes, glm::vec3 color, Shader* shader);
    Model(std::vector<Mesh> meshes, glm::vec3 color, Shader* shader,
          GLfloat* modelMatrixValuePtr);

    void setModelMatrix(GLfloat* modelMatrixValuePtr);
    void setLightColor(glm::vec3 lightColor);

    void draw();
    void drawWith(GLfloat* modelMatrixValuePtr);
    
    static Model createGround(Shader* shader, const int meshCount = MESH_COUNT,
                              const GLfloat maxHillHeight = MAX_HILL_HEIGHT);
    static Model createCube(Shader* shader);
    static Model fromFile(std::string path, glm::vec3 color, Shader* shader);
    
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

#endif /* model_hpp */
