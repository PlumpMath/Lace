#version 330 core

layout (location = 0) in vec2 pos;

void main() {
    mat4 transform = mat4(vec4(1, 0, 0, 0),
                          vec4(0, 1, 0, 0),
                          vec4(0, 0, 1, 0),
                          vec4(0, 0, 0, 1));
    
    gl_Position = transform * vec4(pos.x, pos.y, 0.0, 1.0);
}
