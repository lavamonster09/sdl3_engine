#version 460

struct pointLight {
    vec3 position;
    vec3 color;
    float strength;
};


layout (location = 0) in VS_OUT {
    vec3 frag_pos;
    vec2 uv;
    vec3 view_pos;
    pointLight point_lights[4];
    mat3 TBN;
} fs_in;

layout (location = 0) out vec4 FragColor;

layout (set = 2, binding = 0) uniform sampler2D texSampler;
layout (set = 2, binding = 1) uniform sampler2D normalMap;
layout (set = 2, binding = 2) uniform sampler2D roughnessMap;

vec3 calculate_point_light(pointLight point_light) {
    vec3 normal = texture(normalMap, fs_in.uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 light_direction = fs_in.TBN * normalize(point_light.position - fs_in.frag_pos);
    vec3 view_direction = fs_in.TBN * normalize(fs_in.view_pos - fs_in.frag_pos);
    vec3 halfway_direction = normalize(light_direction + view_direction);

    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diff * vec3(point_light.color);

    float spec = pow(max(dot(normal, halfway_direction), 0.0), texture(roughnessMap, fs_in.uv).x * 8);
    vec3 specular = point_light.color.rgb * spec;
    float distance = length(point_light.position - fs_in.frag_pos);

    return ((vec3(0.1f, 0.1f, 0.1f) + diffuse + specular) * (1 / (distance * distance)) * point_light.strength);
}

vec3 calculate_directional_light(vec3 direction) {
    vec3 normal = texture(normalMap, fs_in.uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 light_direction = fs_in.TBN * normalize(-direction);
    vec3 view_direction = fs_in.TBN * normalize(fs_in.view_pos - fs_in.frag_pos);
    vec3 halfway_direction = normalize(light_direction + view_direction);

    float diff = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);

    float spec = pow(max(dot(normal, halfway_direction), 0.0), texture(roughnessMap, fs_in.uv).x * 8);
    vec3 specular = vec3(0.1f, 0.1f, 0.1f) * spec;

    return ((vec3(0.1f, 0.1f, 0.1f) + diffuse + specular));
}

void main() {
    vec4 t = texture(texSampler, fs_in.uv);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++) {
        result += calculate_point_light(fs_in.point_lights[i]);
    }
    result += calculate_directional_light(vec3(-0.2f, -1.0f, -0.0f));
    if (t.a != 0) {
        float gamma = 2.7;
        FragColor = pow(vec4(result * t.rgb, 1.0f), vec4(1.0 / gamma));;
    }
    else {
        discard;
    }

    //FragColor = vec4(t.rgb, 1.0);
    //FragColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);
}

