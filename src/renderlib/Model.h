//
// Created by oliver on 21/08/2025.
//

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "../engine_types.h"
#include "Camera.h"
#include "Mesh.h"


class Model {
public:
    Model(SDL_GPUDevice *device, std::string path) {
        load_model(device, path);
        upload_buffers(device);
    };

    void draw(SDL_GPURenderPass *render_pass, SDL_GPUSampler *sampler, const Camera *camera,
              SDL_GPUCommandBuffer *command_buffer, const std::vector<PointLight *> &lights);

    void update_scale(float scale);

    void update_pos(glm::vec3 pos);

    void update_rotation(float angle, glm::vec3 axis);

private:
    float rotation = 0.0f;
    glm::vec3 rotation_axis{1.0f, 0.0f, 0.0f};
    glm::vec3 pos{0.0f, 0.0f, 0.0f};
    float scale = 1.0f;
    std::vector<Mesh> meshes;
    glm::mat4 model_mat;

    void upload_buffers(SDL_GPUDevice *device);

    void load_model(SDL_GPUDevice *device, std::string path);

    void process_model(SDL_GPUDevice *device, aiNode *node, const aiScene *scene);

    Mesh process_mesh(SDL_GPUDevice *device, aiMesh *mesh, const aiScene *scene);

    void upload_texture(SDL_GPUCopyPass *copy_pass, SDL_GPUDevice *device, Texture texture);
};


#endif //MODEL_H
