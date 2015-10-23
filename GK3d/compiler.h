//
//  compiler.h
//  GK3d
//
//  Created by Paweł Kobojek on 23.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#ifndef compiler_h
#define compiler_h

#include <GL/glew.h>

namespace glsl {
    void printProgramLog( GLuint program );
    void printShaderLog( GLuint shader );
}

#endif /* compiler_h */
