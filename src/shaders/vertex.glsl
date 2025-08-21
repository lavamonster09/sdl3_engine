#version 460

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 0) out vec4 v_color;

layout (std140, set = 1, binding = 0) uniform UniformBlock {
    mat4 model;
    mat4 view;
    mat4 projection;
} transform_block;

void main() {
    gl_Position = transform_block.projection * transform_block.view * transform_block.model * vec4(a_position, 1.0f);
    v_color = a_color;
}