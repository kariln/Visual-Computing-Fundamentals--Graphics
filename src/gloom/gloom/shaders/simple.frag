
#version 430 core

layout(location = 1) in vec4 color;
out vec4 frag_color;

void main() {
  frag_color = vec4(color);
}
