//
// Created by oliver on 21/08/2025.
//

#ifndef ENGINE_H
#define ENGINE_H

#include "CameraController.h"
#include "ECS.h"
#include "InputManager.h"
#include "Renderer3DSystem.h"
#include "renderlib/renderlib.h"

extern ECS::Coordinator g_coordinator;

class Engine {
    bool should_quit = false;
    Uint64 last_frame = 0.0;
    Uint64 current_frame = SDL_GetPerformanceCounter();
    InputManager *input_manager = nullptr;
    CameraController *camera_controller = nullptr;
    std::shared_ptr<Renderer3DSystem> system;

public:
    Renderer *renderer = nullptr;
    double delta_time = 0.0;
    std::unordered_map<std::string, ECS::Entity> entities;

    void init();

    void game_loop();

    void events();

    template<typename T>
    void add_component(std::string key, T component);

    void add_entity(std::string key);
};

template<typename T>
void Engine::add_component(std::string key, T component) {
    g_coordinator.add_component(entities[key], component);
}


#endif //ENGINE_H
