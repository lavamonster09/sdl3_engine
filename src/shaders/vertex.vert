#version 460

struct pointLight {
    vec3 position;
    vec3 color;
    float strength;
};

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in vec3 a_tangent;

layout (location = 0) out VS_OUT {
    vec3 frag_pos;
    vec2 uv;
    vec3 view_pos;
    pointLight point_lights[4];
    mat3 TBN;
} vs_out;

layout (std140, set = 1, binding = 0) uniform UniformBlock {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec3 view_pos;
    pointLight point_lights[4];
} transform_block;

void main() {
    vec3 T = normalize(vec3(transform_block.model * vec4(a_tangent, 0.0)));
    vec3 N = normalize(vec3(transform_block.model * vec4(a_normal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));

    gl_Position = transform_block.projection * transform_block.view * transform_block.model * vec4(a_position, 1.0f);
    vs_out.frag_pos = vec3(transform_block.model * vec4(a_position, 1.0));
    vs_out.uv = a_uv;
    vs_out.view_pos = transform_block.view_pos;
    vs_out.point_lights = transform_block.point_lights;
    vs_out.TBN = TBN;
}