#version 460

struct pointLight {
    vec3 position;
    vec3 color;
};

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

layout (location = 0) out vec3 v_normal;
layout (location = 1) out vec3 v_frag_pos;
layout (location = 2) out vec2 v_uv;
layout (location = 3) out vec3 view_pos;
layout (location = 4) out pointLight v_point_lights[4];


layout (std140, set = 1, binding = 0) uniform UniformBlock {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec3 view_pos;
    pointLight point_lights[4];
} transform_block;

void main() {
    gl_Position = transform_block.projection * transform_block.view * transform_block.model * vec4(a_position, 1.0f);
    v_normal = a_normal;
    v_frag_pos = vec3(transform_block.model * vec4(a_position, 1.0));
    v_uv = a_uv;
    view_pos = transform_block.view_pos;
    v_point_lights = transform_block.point_lights;
}