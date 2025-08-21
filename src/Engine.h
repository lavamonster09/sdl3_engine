//
// Created by oliver on 21/08/2025.
//

#ifndef ENGINE_H
#define ENGINE_H
#include <SDL3/SDL_video.h>

#include "CameraController.h"
#include "InputManager.h"
#include "Renderer.h"

class CameraController;

class Engine {
    bool should_quit = false;
    Uint64 last_frame = 0.0;
    Uint64 current_frame = SDL_GetPerformanceCounter();
    Renderer renderer{};
    InputManager input_manager{};
    CameraController *camera_controller{};

public:
    double delta_time = 0.0;

    void init();

    void game_loop();

    void events();
};


#endif //ENGINE_H
