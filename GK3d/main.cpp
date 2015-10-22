//
//  main.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 21.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>

#include <GLFW/glfw3.h>

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0, .5);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-.5, -.5);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(.5, -.5);
    glEnd();
}

int main(int argc, const char * argv[]) {
    GLFWwindow* window;
    
    if(!glfwInit()) {
        return -1;
    }
    window = glfwCreateWindow(640, 480, "GK", NULL, NULL);
    
    if(!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    if(!glewInit()) {
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    while(!glfwWindowShouldClose(window)) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}
