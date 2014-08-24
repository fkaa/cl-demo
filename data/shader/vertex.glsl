#version 410

layout(location = 0) in vec4 vertices;

uniform mat4 proj;

void main() {
  gl_Position = proj * vertices;
}
