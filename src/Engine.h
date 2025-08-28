//
// Created by oliver on 21/08/2025.
//

#ifndef ENGINE_H
#define ENGINE_H

#include "CameraController.h"
#include "InputManager.h"
#include "renderlib/renderlib.h"

class Engine {
    bool should_quit = false;
    Uint64 last_frame = 0.0;
    Uint64 current_frame = SDL_GetPerformanceCounter();
    InputManager *input_manager = nullptr;
    CameraController *camera_controller = nullptr;

public:
    Renderer *renderer = nullptr;
    double delta_time = 0.0;

    void init();

    void game_loop();

    void events();
};


#endif //ENGINE_H
