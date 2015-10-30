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
const int POINT_LIGHTS_COUNT = 4;

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

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f,  3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  4.0f, -3.0f)
};

bool polygonMode = false;

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
    if(key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        polygonMode = !polygonMode;
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

void drawLights(Model lightCube, std::vector<glm::mat4> modelMatrices) {
    lightCube.shader->use();
    glm::mat4 view = camera.getViewMatrix();
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH/HEIGHT, 0.1f, 100.0f);
    
    GLuint uniformLocation = glGetUniformLocation(lightCube.shader->program, "view");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));
    uniformLocation = glGetUniformLocation(lightCube.shader->program, "projection");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));
    
    uniformLocation = glGetUniformLocation(lightCube.shader->program, "viewerPos");
    glUniform3f(uniformLocation, camera.position[0], camera.position[1], camera.position[2]);
    for (int i = 0; i < modelMatrices.size(); ++i) {
        lightCube.drawWith(glm::value_ptr(modelMatrices[i]));
    }
}

void render(std::vector<Model> models, Model lightCube, std::vector<glm::mat4> lightModelMatrices) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    drawLights(lightCube, lightModelMatrices);
    
    for (int i = 0; i < models.size(); ++i) {
        models[i].shader->use();
        glm::mat4 view = camera.getViewMatrix();
        
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH/HEIGHT, 0.1f, 100.0f);
        
        GLuint uniformLocation = glGetUniformLocation(models[i].shader->program, "view");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));
        uniformLocation = glGetUniformLocation(models[i].shader->program, "projection");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));
        
        uniformLocation = glGetUniformLocation(models[i].shader->program, "viewerPos");
        glUniform3f(uniformLocation, camera.position[0], camera.position[1], camera.position[2]);
        
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[0].position"), pointLightPositions[0].x,
                    pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[0].linear"), 0.09);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[0].quadratic"), 0.032);
        // Point light 2
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[1].linear"), 0.09);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[1].quadratic"), 0.032);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[2].linear"), 0.09);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[2].quadratic"), 0.032);
        // Point light 4
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[3].linear"), 0.09);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "pointLights[3].quadratic"), 0.032);
        
        // Spotlight
        glUniform3f(glGetUniformLocation(models[i].shader->program, "spotLight.position"), camera.position.x, camera.position.y,
                    camera.position.z);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "spotLight.direction"), camera.front.x, camera.front.y,
                    camera.front.z);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(models[i].shader->program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "spotLight.linear"), 0.09);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "spotLight.quadratic"), 0.032);
        glUniform1f(glGetUniformLocation(models[i].shader->program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(models[i].shader->program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));
        
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
    glEnable(GL_DEPTH_TEST);
    
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
    
    Model justCube = Model::createCube(&shader);
    glm::mat4 justModel;
    justModel = glm::translate(justModel, glm::vec3(1.0f, 1.0f, 0.0f));
    justCube.setModelMatrix(glm::value_ptr(justModel));
    models.push_back(justCube);
    
    Model lightCube = Model::createCube(&lightShader);
    std::vector<glm::mat4> lightModelMatrices;
    for (int i = 0; i < POINT_LIGHTS_COUNT; ++i) {
        glm::mat4 lightModel;
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightModel = glm::translate(lightModel, pointLightPositions[i]);
        lightModelMatrices.push_back(lightModel);
    }
    
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode ? GL_LINE : GL_FILL);
        do_movement();
        render(models, lightCube, lightModelMatrices);
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
