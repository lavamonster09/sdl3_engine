//
// Created by oliver on 21/08/2025.
//

#include "Engine.h"
#include "ECS.h"
#include "Renderer3DSystem.h"

Engine g_engine{};
extern ECS::Coordinator g_coordinator;

int main() {
    g_engine.init();

    g_engine.add_entity("socrates");
    g_engine.add_component("socrates", Transform3D{
                               {0.0f, 0.0f, -0.0f},
                               {1.0f, 0.0f, 0.0f},
                               glm::radians(-90.0f),
                               0.05f
                           });
    g_engine.add_component("socrates", Mesh3D{
                               "../assets/socrates.gltf",
                           });

    g_engine.add_entity("map");
    g_engine.add_component("map", Transform3D{
                               {0.0f, 0.0f, -0.0f},
                               {1.0f, 0.0f, 0.0f},
                               glm::radians(0.0f),
                               0.05f
                           });
    g_engine.add_component("map", Mesh3D{
                               "../assets/scene.gltf"
                           });

    g_engine.renderer->add_light({0.0f, 5.0f, -3.0f}, {1.0f, 0.0f, 0.0f}, true, 1.0f);
    //engine.renderer->add_light({0.0f, 5.0f, 3.0f}, {0.0f, 1.0f, 0.0f}, true, 1.0f);
    //engine.renderer->add_light({3.0f, 5.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, true, 1.0f);
    //engine.renderer->add_light({1000.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, true, 300090.0f);
    g_engine.game_loop();
    return 0;
}

