//
// Created by oliver on 20/08/2025.
//

#include "Camera.h"

Camera::Camera() {
    init();
}

void Camera::update() {
    if (pitch >= 89.5f) {
        pitch = 89.5f;
    } else if (pitch <= -89.5f) {
        pitch = -89.5f;
    }
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
    view = glm::lookAt(pos, pos + front, up);
    projection = glm::perspective(glm::radians(fov), 16.0f / 9.0f, 0.1f, 100.0f);
}

void Camera::init() {
    direction = glm::vec3{1.0f, 0.0f, 0.0f};
    right = glm::normalize((glm::vec3{0.0f, 1.0f, 0.0f}, direction));
    up = glm::vec3{0.0f, 1.0f, 0.0f};
    pos = glm::vec3{0.0f, 0.0f, 3.0f};
    front = glm::vec3(0.0f, 0.0f, -1.0f);
}
