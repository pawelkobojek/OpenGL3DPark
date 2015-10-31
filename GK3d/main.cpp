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
#include "light.hpp"
#include "SpotLight.hpp"

const GLuint WIDTH = 800, HEIGHT = 600;
const int POINT_LIGHTS_COUNT = 2;

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

Light pointLights[] = {
    Light(glm::vec3(2.5f, 0.75f, 0.0f), 1.0f, 0.09, 0.032),
    Light(glm::vec3(0.0f, 0.75f, 3.5f), 1.0f, 0.09, 0.032)
};

bool polygonMode = false;
bool flashLight = true;

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
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        flashLight = !flashLight;
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
    glUniform3f(uniformLocation, camera.position.x, camera.position.y, camera.position.z);
    for (int i = 0; i < modelMatrices.size(); ++i) {
        lightCube.drawWith(glm::value_ptr(modelMatrices[i]));
    }
}

void setupCoordsSystemUniforms(GLuint shaderProgram) {
    glm::mat4 view = camera.getViewMatrix();
    
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH/HEIGHT, 0.1f, 100.0f);
    
    GLuint uniformLocation = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));
    uniformLocation = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));
    
    uniformLocation = glGetUniformLocation(shaderProgram, "viewerPos");
    glUniform3f(uniformLocation, camera.position.x, camera.position.y, camera.position.z);

}

void render(std::vector<Model> models, Model lightCube, std::vector<glm::mat4> lightModelMatrices, Light pointLights[],
            SpotLight spotLight) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    pointLights[0].strength = fabs(sin(glfwGetTime()));
    
    drawLights(lightCube, lightModelMatrices);
    
    for (int i = 0; i < models.size(); ++i) {
        models[i].shader->use();
        
        setupCoordsSystemUniforms(models[i].shader->program);
        for (int j = 0; j < POINT_LIGHTS_COUNT; ++j) {
            pointLights[j].attachTo(models[i].shader->program, j);
        }
        spotLight.setStrength(flashLight ? 1.0f : 0.0f);
        spotLight.setPosition(camera.position);
        spotLight.direction = camera.front;
        spotLight.attachTo(models[i].shader->program, 0);
        
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
    model = glm::scale(model, glm::vec3(20.0f));
    ground.setModelMatrix(glm::value_ptr(model));
    models.push_back(ground);
    
    Model cube = Model::createCube(&shader);
    glm::mat4 justModel;
    justModel = glm::translate(justModel, glm::vec3(0.0f, 2.0f, 0.0f));
    cube.setModelMatrix(glm::value_ptr(justModel));
    models.push_back(cube);
    
    Model lantern = Model::fromFile(std::string("/Users/pawelkobojek/Development/grafika/GK3d/GK3d/lamp-post-ready.3DS"),
                                          glm::vec3(0.8f, 0.8f, 0.8f), &shader);
    glm::mat4 mm;
    
    glm::vec3 lanternPos[] = {
        glm::vec3(pointLights[0].position.x, pointLights[0].position.y * 2.0f, pointLights[0].position.z),
        glm::vec3(pointLights[1].position.x, pointLights[1].position.y * 2.0f, pointLights[1].position.z),
    };
    std::vector<glm::mat4> lightModelMatrices;
    for (int i = 0; i < POINT_LIGHTS_COUNT; ++i) {
        glm::mat4 lightModel;
        lightModel = glm::translate(lightModel, lanternPos[i]);
        lightModel = glm::scale(lightModel, glm::vec3(0.02f));
        lightModelMatrices.push_back(lightModel);
    }
    
    Model bench = Model::fromFile(std::string("/Users/pawelkobojek/Development/grafika/GK3d/GK3d/bench.3ds"), glm::vec3(0.65f, 0.2f, 0.2f), &shader);
    glm::mat4 m2;
    m2 = glm::translate(m2, glm::vec3(0.0f, 0.1f, 0.0f));
    m2 = glm::rotate(m2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m2 = glm::scale(m2, glm::vec3(0.002f));
    bench.setModelMatrix(glm::value_ptr(m2));
    models.push_back(bench);
    
    SpotLight flashLight(camera.position, camera.front, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)),
                         1.0f, 0.09, 0.032);
    
    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode ? GL_LINE : GL_FILL);
        do_movement();
        render(models, lantern, lightModelMatrices, pointLights, flashLight);
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
