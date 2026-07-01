#version 460 core
out vec4 FragColor;

struct Light {
    vec3 direction;
    vec4 color;
    float intensity;
};

uniform Light u_main_light;
uniform float u_time;

uniform sampler2D main_tex;
uniform vec4 base_color;

in vec2 uv;
in vec3 normal;

void main()
{
    float light_factor = (1 - ((dot(u_main_light.direction, normal) + 1.0)/2.0));
    vec4 light_color = light_factor * u_main_light.color * u_main_light.intensity;
    FragColor = texture(main_tex, uv) * base_color * light_color;
}