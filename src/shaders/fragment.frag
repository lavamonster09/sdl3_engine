#version 460

struct pointLight {
    vec3 position;
    vec3 color;
};


layout (location = 0) in vec3 v_normal;
layout (location = 1) in vec3 v_frag_pos;
layout (location = 2) in vec2 v_uv;
layout (location = 3) in vec3 view_pos;
layout (location = 4) in pointLight v_point_lights[4];

layout (location = 0) out vec4 FragColor;

layout (set = 2,binding = 0) uniform sampler2D texSampler;
layout (set = 2,binding = 1) uniform sampler2D normalMap;

vec3 calculate_point_light(pointLight point_light) {
    vec3 normal = texture(normalMap, v_uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 light_direction = normalize(point_light.position - v_frag_pos);
    vec3 view_direction = normalize(view_pos - v_frag_pos);
    vec3 halfway_direction = normalize(light_direction + view_direction);

    float diff = max(dot(v_normal, light_direction), 0.0);
    vec3 diffuse = diff * vec3(point_light.color);

    float spec = pow(max(dot(v_normal, halfway_direction), 0.0), 32.0);
    vec3 specular = point_light.color.rgb * spec;

    return ( vec3(0.1f, 0.1f, 0.1f) + diffuse + specular);
}

void main() {
    vec4 t = texture(texSampler, v_uv);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++) {
        result += calculate_point_light(v_point_lights[i]);
    }
    FragColor = vec4(result * t.rgb, 1.0);
}

