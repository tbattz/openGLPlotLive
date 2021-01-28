#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;

out vec3 fColor;

uniform mat4 transformViewport;
uniform uint logYBase;

void main() {
    float logyOffset = log(aOffset.y) / log(logYBase);
    gl_Position = vec4(aPos, 0.0, 0.0) + (transformViewport * vec4(aOffset.x, logyOffset, 0.5, 1.0));
}

