//
//  shader.hpp
//  GK3d
//
//  Created by Paweł Kobojek on 24.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader {
    const GLchar* vertexPath;
    const GLchar* fragmentPath;
public:
    GLuint program;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void readAndCompile();
    void use();
};

#endif /* shader_hpp */
