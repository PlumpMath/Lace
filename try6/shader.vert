#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 col;

uniform mat4 transform;

out vec3 passed_color;

void main() {
    // transform = mat4(
    //     vec4(0.9, 0.8,   0,   0),
    //     vec4(0.0, 0.9, 0,   0.8),
    //     vec4(0, 0,   0.8, 0),
    //     vec4(0, 0,   0,   0.95)
    //     );
    passed_color = col;
    gl_Position = transform * vec4(pos.x, pos.y, pos.y, 1.0);
}
