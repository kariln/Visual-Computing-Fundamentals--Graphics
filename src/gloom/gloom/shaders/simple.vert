
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vertex_color;
uniform mat4 matrix;

out vec4 color;

void main()
{
    color = vertex_color;
    gl_Position = matrix*position;


}
