//
// Created by oliver on 30/09/2025.
//

#include "Engine.h"
#include "engine_types.h"

void Engine::init() {
    renderer_.init();
}

void Engine::start() {
    while (!should_quit_) {
        events_();
        renderer_.draw();
    }
}

void Engine::set_config(const Config &config) {
    renderer_.create_window(config.window_create_info);
}

void Engine::events_() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            should_quit_ = true;
        }
    }
}
