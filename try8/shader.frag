#version 330 core

in vec2 vPos;
out vec4 color;

void main() {
    color = vec4(0.5 + vPos.x, 0.9 + sin(vPos.y) * 0.3, 0.6, 1.0);
}
