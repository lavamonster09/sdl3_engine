//
// Created by oliver on 21/08/2025.
//

#include "Engine.h"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>

#include <iostream>

void Engine::init() {
    camera_controller = new CameraController();
    renderer = new Renderer();
    input_manager = new InputManager();
    renderer->init();
}

void Engine::game_loop() {
    while (!should_quit) {
        last_frame = current_frame;
        current_frame = SDL_GetPerformanceCounter();
        delta_time = (double) ((current_frame - last_frame) / (double) SDL_GetPerformanceFrequency());
        events();
        camera_controller->input(input_manager);
        camera_controller->update();
        renderer->draw(camera_controller);
    }
}

void Engine::events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            should_quit = true;
        }
        input_manager->handle_event(&event);
    }
}