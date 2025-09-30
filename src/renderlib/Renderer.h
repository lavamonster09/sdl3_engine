//
// Created by oliver on 21/08/2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <AssetManager.h>
#include <queue>
#include <unordered_map>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

#include "Camera.h"
#include "../engine_types.h"
#include "Model.h"

class Renderer {
public:
    std::queue<Model *> draw_queue;
    SDL_Window *window = nullptr;

    void init();

    void draw(Camera *camera);

    Model *add_model(std::string &model_path);

    void add_light(glm::vec3 position, glm::vec3 color, bool billboard, float strength);

private:
    AssetManager asset_manager;
    glm::vec4 clear_color;
    SDL_GPUDevice *device = nullptr;
    SDL_GPUShader *vertex_shader = nullptr;
    SDL_GPUShader *fragment_shader = nullptr;
    SDL_GPUGraphicsPipeline *graphics_pipeline = nullptr;
    ModelUniformBuffer uniform_buffer = {};
    SDL_GPUTexture *depth_texture = nullptr;
    SDL_GPUSampler *sampler;
    std::vector<PointLight *> point_lights;
};


#endif //RENDERER_H
