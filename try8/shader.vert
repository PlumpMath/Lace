#version 330 core

layout (location = 0) in vec2 pos;

uniform float t;

out vec2 vPos;

void main() {
    mat4 transform = mat4(vec4(1, 0.2, 0, 0),
                          vec4(-0.2, 1, 0, 0),
                          vec4(0, 0, 1, 0),
                          vec4(0, 0, 0, 1));
    vPos = pos;
    gl_Position = transform * vec4(pos.x, pos.y + sin(t + pos.x) * 0.15, 0.0, 1.0);
}
