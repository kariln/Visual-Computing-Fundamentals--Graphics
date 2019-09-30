
#version 430 core

out vec4 color;

void main()
{
    float val = (gl_FragCoord.z);
    color = vec4(val, 0.0f, 0.0f, 1.0f);
}
