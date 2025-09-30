//
// Created by oliver on 30/09/2025.
//

#ifndef ENGINE_H
#define ENGINE_H
#include "engine_types.h"
#include "Renderer.h"


class Engine {
public:
    void init();

    void set_config(const Config &config);

    void start();

private:
    Renderer renderer_;
    bool should_quit_;

    void events_();
};

#endif //ENGINE_H
