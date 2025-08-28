//
// Created by oliver on 21/08/2025.
//

#include "Engine.h"

Engine engine{};

int main() {
    engine.init();
    std::vector<ModelCreateInfo> info;
    info.push_back( {"car", "../assets/Socrates.obj", "../assets/1001_Base_Color.bmp", "../assets/1001_Normal_OpenGL.bmp", "../assets/1001_Roughness.bmp"});
    info.push_back( {"floor", "../assets/Floor.obj", "../assets/ground_diff.bmp", "../assets/ground_norm.bmp","../assets/ground_rough.bmp"});
    engine.renderer->add_models(info);
    engine.renderer->models["car"]->update_scale(0.05f);
    engine.renderer->models["floor"]->update_scale(2.0f);
    engine.renderer->add_light({0.0f, 5.0f, -3.0f}, {1.0f, 0.0f, 0.0f}, true, 1.0f);
    engine.renderer->add_light({0.0f, 5.0f, 3.0f}, {0.0f, 1.0f, 0.0f}, true, 1.0f);
    engine.renderer->add_light({3.0f, 5.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, true, 1.0f);
    engine.renderer->add_light({3.0f, 40.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, true, 30.0f);
    engine.game_loop();
    return 0;
}

