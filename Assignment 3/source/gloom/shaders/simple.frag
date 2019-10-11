#version 430 core

in vec4 color;
out vec4 frag_color;

in vec3 normal;
out vec3 light;

vec3 lightDirection  = normalize(vec3(0.8, -0.5, 0.6));


void main() {
  //frag_color = vec4(color);
  //frag_color = vec4(normal,1);

  frag_color = vec4(color.xyz * (max(0,dot(normal,-lightDirection))),color.w);

}
