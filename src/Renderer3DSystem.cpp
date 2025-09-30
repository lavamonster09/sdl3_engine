//
// Created by oliver on 18/09/2025.
//

#include "Renderer3DSystem.h"
#include "Engine.h"

extern ECS::Coordinator g_coordinator;
extern Engine g_engine;

void Renderer3DSystem::start() {
    for (auto const &entity: m_entities) {
        auto &transform = g_coordinator.get_component<Transform3D>(entity);
        auto &mesh = g_coordinator.get_component<Mesh3D>(entity);

        auto model = g_engine.renderer->add_model(mesh.model_path);
        mesh.model = model;

        model->update_pos(transform.position);
        model->update_rotation(transform.rotation, transform.rotation_axis);
        model->update_scale(transform.scale);
    }
}

void Renderer3DSystem::update() {
    for (auto const &entity: m_entities) {
        auto &transform = g_coordinator.get_component<Transform3D>(entity);
        auto &mesh = g_coordinator.get_component<Mesh3D>(entity);

        g_engine.renderer->draw_queue.push(mesh.model);

        mesh.model->update_pos(transform.position);
        mesh.model->update_rotation(transform.rotation, transform.rotation_axis);
        mesh.model->update_scale(transform.scale);
    }
}
