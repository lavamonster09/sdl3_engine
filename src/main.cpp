//
// Created by oliver on 21/08/2025.
//

#include "Engine.h"

Engine engine{};

int main() {
    engine.init();
    std::vector<ModelCreateInfo> info;
    info.push_back({
        "car", "../assets/socrates.gltf"
    });
    info.push_back({
        "floor", "../assets/scene.gltf"
    });
    info.push_back({
        "light", "../assets/Sphere.obj"
    });
    engine.renderer->add_models(info);
    engine.renderer->models["car"]->update_scale(0.05f);
    engine.renderer->models["car"]->update_rotation(glm::radians(-90.0f), glm::vec3{1.0f, 0.0f, 0.0f});
    engine.renderer->models["floor"]->update_scale(0.05f);
    engine.renderer->models["floor"]->update_rotation(glm::radians(180.0f), glm::vec3{0.0f, 0.0f, 1.0f});
    engine.renderer->models["light"]->update_pos({0.0f, 5.0f, -3.0f});
    engine.renderer->add_light({0.0f, 5.0f, -3.0f}, {1.0f, 0.0f, 0.0f}, true, 1.0f);
    engine.renderer->add_light({0.0f, 5.0f, 3.0f}, {0.0f, 1.0f, 0.0f}, true, 1.0f);
    engine.renderer->add_light({3.0f, 5.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, true, 1.0f);
    engine.renderer->add_light({1000.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, true, 300090.0f);
    engine.game_loop();
    return 0;
}

