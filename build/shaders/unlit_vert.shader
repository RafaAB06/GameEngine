
#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec4 gl_Position;
out vec3 normal;
out vec2 uv;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_position.x, a_position.y, a_position.z, 1.0);

    mat3 normal_matrix = transpose(inverse(mat3(u_model)));
    normal = normalize(normal_matrix * a_normal);
    uv = a_uv;
}