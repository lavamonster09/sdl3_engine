//
// Created by oliver on 27/08/2025.
//

#ifndef SDL3_ENGINE_MESH_H
#define SDL3_ENGINE_MESH_H
#include <vector>
#include <SDL3/SDL_gpu.h>

#include "camera.h"
#include "engine_types.h"


class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<Uint32> indices;

    Mesh(SDL_GPUDevice *device, std::vector<Vertex> vertices, std::vector<Uint32> indices);
    void draw(SDL_GPURenderPass *render_pass, SDL_GPUSampler *sampler, const Camera &camera, SDL_GPUCommandBuffer *command_buffer, const std::vector<PointLight *> &lights, glm::mat4 model_mat);
    void upload_mesh(SDL_GPUCommandBuffer *command_buffer, SDL_GPUCopyPass *copy_pass, SDL_GPUDevice *device);
private:
    MeshBuffer mesh_buffer;

    void setup_mesh(SDL_GPUDevice *device);
};


#endif //SDL3_ENGINE_MESH_H