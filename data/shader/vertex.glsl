#version 410

layout(location = 0) in vec2 vertices;

uniform mat4 proj;

void main() {
  gl_Position = proj * vec4(vertices, 0.f, 1.f);
}
