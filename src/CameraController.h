//
// Created by oliver on 21/08/2025.
//

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H
#include "Camera.h"
#include "InputManager.h"
#include "Engine.h"

class Engine;

class CameraController {
public:
    Camera camera;

    CameraController() {
        camera = Camera();
    }

    void input(Engine *engine, InputManager &input_manager);
};


#endif //CAMERACONTROLLER_H
