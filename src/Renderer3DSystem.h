//
// Created by oliver on 18/09/2025.
//

#ifndef RENDERER3DSYSTEM_H
#define RENDERER3DSYSTEM_H

#include "ECS.h"

class Renderer3DSystem : public ECS::System {
public:
    void start();

    void update();
};


#endif //RENDERER3DSYSTEM_H
