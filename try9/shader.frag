#version 330 core

in vec3 pos;

out vec4 color;

void main() {
    int ri = int((0.8 + sin(pos.x) * 0.2) * 20);
    int gi = int((0.3 + sin(pos.y) * 0.2) * 20);
    color = vec4(ri / 20.0, gi / 20.0, 0.4, 1.0);
}
