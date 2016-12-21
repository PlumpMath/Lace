#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 col;

out vec3 passed_color;

void main() {
    passed_color = col;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
}
