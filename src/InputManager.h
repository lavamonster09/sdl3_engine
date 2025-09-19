//
// Created by oliver on 20/08/2025.
//

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <SDL3/SDL_events.h>
#include "glm/glm.hpp"


class InputManager {
public:
    bool key_down[368];
    bool key_just_pressed[368];
    float dx, dy;
    glm::vec2 mouse_pos = glm::vec2{0.0f, 0.0f};
    glm::vec2 last_mouse_pos = glm::vec2{0.0f, 0.0f};

    void handle_event(SDL_Event *e);
};


#endif //INPUT_MANAGER_H
