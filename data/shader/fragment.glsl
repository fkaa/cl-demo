#version 410

out vec4 color;

void main() {
  vec2 pos = gl_FragCoord.xy;
  
  color = vec4(pos.x, pos.y, pos.x, 0.1);
}
