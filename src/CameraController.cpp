//
// Created by oliver on 21/08/2025.
//

#include "CameraController.h"

void CameraController::input(Engine *engine, InputManager &input_manager) {
    camera.pitch -= input_manager.dy * 0.3;
    camera.yaw += input_manager.dx * 0.3;
    input_manager.dx = 0;
    input_manager.dy = 0;

    camera.update();

    if (input_manager.key_down[97]) {
        glm::vec3 temp = glm::normalize(glm::cross(camera.front, camera.up));
        temp.y = 0;
        camera.pos -= glm::normalize(temp) * camera.speed;
    }
    if (input_manager.key_down[100]) {
        glm::vec3 temp = glm::normalize(glm::cross(camera.front, camera.up));
        temp.y = 0;
        camera.pos += glm::normalize(temp) * camera.speed;
    }
    if (input_manager.key_down[119]) {
        glm::vec3 temp = camera.speed * camera.front;
        temp.y = 0;
        camera.pos += glm::normalize(temp) * camera.speed;
    }
    if (input_manager.key_down[115]) {
        glm::vec3 temp = camera.speed * camera.front;
        temp.y = 0;
        camera.pos -= glm::normalize(temp) * camera.speed;
    }
    if (input_manager.key_down[32]) {
        camera.pos += glm::normalize(glm::vec3{0.0f, 1.0f, 0.0f}) * camera.speed;
    }
    if (input_manager.key_down[296]) {
        camera.pos -= glm::normalize(glm::vec3{0.0f, 1.0f, 0.0f}) * camera.speed;
    }
}
