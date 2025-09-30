//
// Created by oliver on 18/09/2025.
//

#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <Model.h>
#include <glm/glm.hpp>


struct Transform3D {
    glm::vec3 position;
    glm::vec3 rotation_axis;
    float rotation;
    float scale;
};

struct Mesh3D {
    std::string model_path;
    Model *model = nullptr;
};

#endif //COMPONENTS_H
