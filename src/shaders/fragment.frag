#version 460

layout (location = 0) in vec3 v_normal;
layout (location = 1) in vec3 v_frag_pos;
layout (location = 2) in vec3 v_light_pos;
layout (location = 0) out vec4 FragColor;

void main() {
    vec3 normal = normalize(v_normal);
    vec3 light_direction = normalize(v_light_pos - v_frag_pos);
    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);

    vec3 result = (vec3(0.1f, 0.1f, 0.1f) + diffuse) * vec3(1.0f, 0.5f, 0.5f);
    FragColor = vec4(result, 1.0);
}