//
// Created by oliver on 30/09/2025.
//

#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

struct Color {
    float r;
    float g;
    float b;
    float a;
};

struct WindowCreateInfo {
    Color clear_color;
    const char *window_name;
    int w;
    int h;
};

enum ShaderType {
    VERTEX_SHADER,
    FRAGMENT_SHADER
};

enum Result {
    FAIL,
    SUCCESS
};

struct Config {
    WindowCreateInfo &window_create_info;
};

#endif //ENGINE_TYPES_H
