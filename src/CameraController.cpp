//
// Created by oliver on 21/08/2025.
//

#include "CameraController.h"

CameraController::CameraController() {
}

void CameraController::input(InputManager *input_manager) {
    if (!input_manager->key_down[297]) {
        pitch -= input_manager->dy * 0.3;
        yaw += input_manager->dx * 0.3;
    }
    input_manager->dx = 0;
    input_manager->dy = 0;

    update();

    if (input_manager->key_down[97]) {
        glm::vec3 temp = glm::normalize(glm::cross(front, up));
        temp.y = 0;
        pos -= glm::normalize(temp) * speed;
    }
    if (input_manager->key_down[100]) {
        glm::vec3 temp = glm::normalize(glm::cross(front, up));
        temp.y = 0;
        pos += glm::normalize(temp) * speed;
    }
    if (input_manager->key_down[119]) {
        glm::vec3 temp = speed * front;
        temp.y = 0;
        pos += glm::normalize(temp) * speed;
    }
    if (input_manager->key_down[115]) {
        glm::vec3 temp = speed * front;
        temp.y = 0;
        pos -= glm::normalize(temp) * speed;
    }
    if (input_manager->key_down[32]) {
        pos += glm::normalize(glm::vec3{0.0f, 1.0f, 0.0f}) * speed;
    }
    if (input_manager->key_down[296]) {
        pos -= glm::normalize(glm::vec3{0.0f, 1.0f, 0.0f}) * speed;
    }
}
