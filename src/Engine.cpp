//
// Created by oliver on 21/08/2025.
//

#include "Engine.h"

#include <iostream>

void Engine::init() {
    camera_controller = new CameraController();
    renderer.init();
    renderer.add_model("car", "../assets/Socrates.obj", "../assets/1001_Base_Color.bmp", "../assets/1001_Normal_OpenGL.bmp");
    renderer.models["car"]->update_scale(0.05f);
    renderer.add_model("floor", "../assets/Floor.obj", "../assets/ground_diff.bmp", "../assets/ground_norm.bmp");
    renderer.models["floor"]->update_scale(2.0f);
    renderer.add_model("sphere", "../assets/Sphere.obj", "../assets/ground_diff.bmp", "../assets/ground_norm.bmp");
    renderer.models["sphere"]->update_pos({0.0f, 5.0f, 3.0f});
    renderer.add_model("sphere2", "../assets/Sphere.obj", "../assets/ground_diff.bmp", "../assets/ground_norm.bmp");
    renderer.models["sphere2"]->update_pos({0.0f, 5.0f, -3.0f});
    renderer.add_light({0.0f, 5.0f, -3.0f}, {1.0f, 0.0f, 0.0f}, true);
    renderer.add_light({0.0f, 5.0f, 3.0f}, {0.0f, 1.0f, 0.0f}, true);
    renderer.add_light({3.0f, 5.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, true);
}

void Engine::game_loop() {
    while (!should_quit) {
        last_frame = current_frame;
        current_frame = SDL_GetPerformanceCounter();
        delta_time = (double) ((current_frame - last_frame) / (double) SDL_GetPerformanceFrequency());
        events();
        camera_controller->input(this, input_manager);
        renderer.draw(camera_controller->camera);
    }
}

void Engine::events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            should_quit = true;
        }
        input_manager.handle_event(&event);
    }
}
