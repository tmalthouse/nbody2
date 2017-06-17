#version 150

in vec2 position

void main() {
  glPosition = vec4(position, 0.0, 1.0);
}
