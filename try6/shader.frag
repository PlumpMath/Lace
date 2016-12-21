#version 330 core

in vec3 passed_color;

out vec4 color;

void main() {
    color = vec4(passed_color.rbg * vec3(1.0, 1.0, 1.0), 1.0);
}
