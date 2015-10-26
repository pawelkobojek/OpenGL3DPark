//
//  model.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 26.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include "model.hpp"

Model::Model(std::vector<glm::vec3> vertices, std::vector<GLuint> indices) : vertices(vertices), indices(indices) {}

void Model::draw() {
    
}