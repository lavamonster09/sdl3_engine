//
// Created by oliver on 21/08/2025.
//

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H
#include "renderlib/Camera.h"
#include "InputManager.h"

class Engine;

class CameraController: public Camera{
public:
    float speed = 0.05f;

    CameraController();

    void input(InputManager *input_manager);
};


#endif //CAMERACONTROLLER_H
