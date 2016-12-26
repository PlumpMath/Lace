#version 330 core

layout (location = 0) in vec3 vPos;

uniform mat4 cameraPersp;
uniform mat4 transform;

out vec3 pos;

void main() {
    pos = vPos;
    gl_Position = cameraPersp * transform * vec4(vPos.x, vPos.y, vPos.z, 1.0);
}
