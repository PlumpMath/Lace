#version 330 core

layout (location = 0) in vec3 vPos;

uniform mat4 combinedTransform;

out vec3 pos;

void main() {
    pos = vPos;
    gl_Position = combinedTransform * vec4(vPos.x, vPos.y, vPos.z, 1.0);
}
