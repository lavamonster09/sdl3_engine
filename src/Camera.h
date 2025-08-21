//
// Created by oliver on 20/08/2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <SDL3/SDL.h>

class Camera {
public:
    glm::vec3 direction = glm::vec3{1.0f, 0.0f, 0.0f};
    glm::vec3 right = glm::normalize((glm::vec3{0.0f, 1.0f, 0.0f}, direction));
    glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f};
    glm::vec3 pos = glm::vec3{0.0f, 0.0f, 3.0f};
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

    float pitch, yaw = -90, roll;
    float fov = 100.0f;
    float speed = 0.05f;

    void update();

    void init();
};


#endif //CAMERA_H
