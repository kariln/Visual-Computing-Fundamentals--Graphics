#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec3 vertex_normal;


layout(location = 0) uniform mat4 model_matrix;
layout(location = 1) uniform mat4 matrix;

out vec4 color;
out vec3 normal;

void main()
{
    color = vertex_color;
    normal = normalize(mat3(model_matrix)*vertex_normal);
    gl_Position = matrix*vec4(position, 1.0f);
}
