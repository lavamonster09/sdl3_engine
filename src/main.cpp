//
// Created by oliver on 30/09/2025.
//
#include <iostream>
#include "Engine.h"

int main() {
    Engine engine;

    WindowCreateInfo window_create_info{
        {0.0f, 0.0f, 0.0f, 0.0f},
        "engine",
        1600,
        900
    };
    Config config = {
        window_create_info
    };
    engine.set_config(config);
    engine.init();

    engine.start();
    return 0;
}
