//
// Created by oliver on 20/08/2025.
//

#include "InputManager.h"

#include <iostream>

void InputManager::handle_event(SDL_Event *e) {
        if (e->type == SDL_EVENT_KEY_DOWN) {
                if (e->key.key < 127) {
                        key_just_pressed[e->key.key] = true;
                        key_down[e->key.key] = true;
                } else {
                        key_just_pressed[e->key.key - 1073741753] = true;
                        key_down[e->key.key - 1073741753] = true;
                }
        }
        if (e->type == SDL_EVENT_KEY_UP) {
                if (e->key.key < 127) {
                        key_down[e->key.key] = false;
                } else {
                        key_down[e->key.key - 1073741753] = false;
                }
        }
        if (e->type == SDL_EVENT_MOUSE_MOTION) {
                dx = e->motion.xrel;
                dy = e->motion.yrel;
        }
}
