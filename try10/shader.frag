#version 330 core

in vec3 pos;
flat in int i;

out vec4 color;

void main() {
    color = vec4(0.5 + 0.3 * sin(i / 100), 0.5 + 0.3 * sin(i / 15), 0.5 + 0.3 * sin(i / 40), 1.0);
}
