#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 camera;

out vec3 passed_color;

void main() {
    // transform = mat4(
    //     vec4(0.9, 0.8,   0,   0),
    //     vec4(0.0, 0.9, 0,   0.8),
    //     vec4(0, 0,   0.8, 0),
    //     vec4(0, 0,   0,   0.95)
    //     );
    passed_color = col;
    gl_Position = projection * camera *  transform * vec4(pos.x, pos.y, pos.z, 1.0);
}
