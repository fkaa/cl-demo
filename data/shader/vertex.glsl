#version 430

layout(location = 0) in vec4 prev;
layout(location = 1) in vec4 curr;

layout(location = 2) uniform mat4 mvp;

void main() {
  gl_Position = mvp * curr;
}
