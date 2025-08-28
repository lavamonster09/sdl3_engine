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
    Camera();
    float pitch, yaw = -90, roll;
    float fov = 100.0f;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 pos;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 front;

    void update();
    void init();
};


#endif //CAMERA_H
