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

void render(GLuint VAO, Shader shader) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    shader.use();
    
    glm::mat4 model;
    model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
    
    glm::mat4 view = camera.getViewMatrix();
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH/HEIGHT, 0.1f, 100.0f);
    
    glBindVertexArray(VAO);
    GLuint uniformLocation = glGetUniformLocation(shader.program, "model");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(model));
    uniformLocation = glGetUniformLocation(shader.program, "view");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));
    uniformLocation = glGetUniformLocation(shader.program, "projection");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glDrawElements(GL_TRIANGLES, 100 * 100 * 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
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
    
    std::vector<glm::vec3> groundVertices;
    std::vector<GLint> groundIndices;
    const int meshCount = 100;
    
    float from = -1.0f;
    float to = 1.0f;
    float diff = to - from;
    for (int i = 0; i <= meshCount; ++i) {
        for (int j = 0; j <= meshCount; ++j) {
            float x = (i * diff / meshCount) + from;
            float z = (j * diff / meshCount) + from;
            float y = (float) rand() / (float) RAND_MAX;
            groundVertices.push_back(glm::vec3(x, y * 0.05f, z));
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

    Shader shader = Shader("/Users/pawelkobojek/Development/grafika/GK3d/GK3d/shader.vert",
                           "/Users/pawelkobojek/Development/grafika/GK3d/GK3d/shader.frag");
    shader.readAndCompile();
    
    GLuint VAO, VBO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, groundVertices.size() * sizeof(glm::vec3), groundVertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, groundIndices.size() * sizeof(GLint), groundIndices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        do_movement();
        render(VAO, shader);
        glfwSwapBuffers(window);
    }
    
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    
    return 0;
}
