#version 410

out vec4 color;

void main() {
  vec2 pos = gl_FragCoord.xy;
  color = vec4(0.6f, 1.f, 0.7f, 0.025f);
}
