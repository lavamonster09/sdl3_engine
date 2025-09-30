//
// Created by oliver on 21/08/2025.
//

#include "Engine.h"
#include "ECS.h"

#include <imgui_impl_sdl3.h>
#include <iostream>

#include "Renderer3DSystem.h"

ECS::Coordinator g_coordinator;

void Engine::init() {
    camera_controller = new CameraController();
    renderer = new Renderer();
    input_manager = new InputManager();
    renderer->init();
    g_coordinator.init();

    g_coordinator.register_component<Transform3D>();
    g_coordinator.register_component<Mesh3D>();

    system = g_coordinator.register_system<Renderer3DSystem>();

    ECS::Signature signature;
    signature.set(g_coordinator.get_component_type<Transform3D>());
    signature.set(g_coordinator.get_component_type<Mesh3D>());
    g_coordinator.set_system_signature<Renderer3DSystem>(signature);
}

void Engine::game_loop() {
    system->start();
    while (!should_quit) {
        last_frame = current_frame;
        current_frame = SDL_GetPerformanceCounter();
        delta_time = (double) ((current_frame - last_frame) / (double) SDL_GetPerformanceFrequency());
        if (input_manager->key_just_pressed[297]) {
            SDL_SetWindowRelativeMouseMode(renderer->window, !SDL_GetWindowRelativeMouseMode(renderer->window));
            SDL_SetWindowMouseGrab(renderer->window, !SDL_GetWindowMouseGrab(renderer->window));
        }
        events();
        system->update();
        if (SDL_GetWindowMouseGrab(renderer->window)) {
            camera_controller->input(input_manager);
        }
        camera_controller->update();
        renderer->draw(camera_controller);
    }
}

void Engine::events() {
    SDL_Event event;
    for (int i = 0; i < 368; i++) {
        input_manager->key_just_pressed[i] = false;
    }
    while (SDL_PollEvent(&event)) {
        if (!SDL_GetWindowMouseGrab(renderer->window)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
        }
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            should_quit = true;
        }
        input_manager->handle_event(&event);
    }
}

void Engine::add_entity(std::string key) {
    entities[key] = g_coordinator.create_entity();
}
