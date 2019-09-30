
#version 430 core

in vec3 position;

void main()
{
    vec3 new_position = position * vec3(-1, -1, 1);

    gl_Position = vec4(new_position, 1.0f);
}
