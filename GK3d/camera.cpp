//
//  camera.cpp
//  GK3d
//
//  Created by Paweł Kobojek on 24.10.2015.
//  Copyright © 2015 Paweł Kobojek. All rights reserved.
//

#include "camera.hpp"

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY) {
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY) {
    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime) {
    GLfloat velocity = this->movementSpeed * deltaTime;
    if (direction == FORWARD) {
        this->position += this->front * velocity;
    }
    if (direction == BACKWARD) {
        this->position -= this->front * velocity;
    }
    if (direction == LEFT) {
        this->position -= this->right * velocity;
    }
    if (direction == RIGHT) {
        this->position += this->right * velocity;
    }
    if(direction == UP) {
        this->position += this->up * velocity;
    }
    if (direction == DOWN) {
        this->position -= this->up * velocity;
    }
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;
    
    this->yaw   += xoffset;
    this->pitch += yoffset;
    
    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (this->pitch > 89.0f) {
            this->pitch = 89.0f;
        }
        if (this->pitch < -89.0f) {
            this->pitch = -89.0f;
        }
    }
    
    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->up    = glm::normalize(glm::cross(this->right, this->front));
}
