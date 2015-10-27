//
//  main.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 21.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"

const GLuint WIDTH = 800, HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));

bool keys[1024];
bool firstMouse = true;
GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void do_movement() {
    if(keys[GLFW_KEY_W]) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if(keys[GLFW_KEY_S]) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    if(keys[GLFW_KEY_A]) {
        camera.processKeyboard(LEFT, deltaTime);
    }
    if(keys[GLFW_KEY_D]) {
        camera.processKeyboard(RIGHT, deltaTime);
    }
    if(keys[GLFW_KEY_R]) {
        camera.processKeyboard(UP, deltaTime);
    }
    if(keys[GLFW_KEY_F]) {
        camera.processKeyboard(DOWN, deltaTime);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // Window should close if user pressed ESC key
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(key >= 0 && key < 1024) {
        if(action == GLFW_PRESS) {
            keys[key] = true;
        } else if(action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xpos - lastX;
    GLfloat yOffset = lastY - ypos; // reversed, since y is from bottom to top
    lastX = xpos;
    lastY = ypos;
    
    camera.processMouseMovement(xOffset, yOffset);
}

void render(std::vector<Model> models, Shader shader) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < models.size(); ++i) {
        models[i].shader->use();
        glm::mat4 view = camera.getViewMatrix();
        
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH/HEIGHT, 0.1f, 100.0f);
        
        GLuint uniformLocation = glGetUniformLocation(shader.program, "view");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));
        uniformLocation = glGetUniformLocation(shader.program, "projection");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));
        
        models[i].draw();
    }
}

GLFWwindow* initializeGLFWWindow() {
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GK", NULL, NULL);
    if(window == NULL) {
        std::cerr << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    return window;
}

int main(int argc, const char * argv[]) {
    GLFWwindow* window = initializeGLFWWindow();
    if(!window) {
        return -1;
    }
    
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    
    Shader shader = Shader("/Users/pawelkobojek/Development/grafika/GK3d/GK3d/objectShader.vert",
                           "/Users/pawelkobojek/Development/grafika/GK3d/GK3d/objectShader.frag");
    shader.readAndCompile();
    
    Shader lightShader = Shader("/Users/pawelkobojek/Development/grafika/GK3d/GK3d/objectShader.vert",
                                "/Users/pawelkobojek/Development/grafika/GK3d/GK3d/lightShader.frag");
    lightShader.readAndCompile();
    
    
    std::vector<Model> models;
    Model ground = Model::createGround(&shader);
    glm::mat4 model;
    model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
    ground.setModelMatrix(glm::value_ptr(model));
    models.push_back(ground);
    
    Model lightCube = Model::createCube(&lightShader);
    glm::mat4 lightModel;
    lightModel = glm::scale(lightModel, glm::vec3(0.08f));
    lightModel = glm::translate(lightModel, glm::vec3(0.0f, 18.0f, 0.0f));
    lightCube.setModelMatrix(glm::value_ptr(lightModel));
    models.push_back(lightCube);
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        do_movement();
        render(models, shader);
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
