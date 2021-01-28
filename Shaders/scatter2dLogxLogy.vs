#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;

out vec3 fColor;

uniform mat4 transformViewport;
uniform uint logXBase;
uniform uint logYBase;

void main() {
    float logxOffset = log(aOffset.x) / log(logXBase);
    float logyOffset = log(aOffset.y) / log(logYBase);
    gl_Position = vec4(aPos, 0.0, 0.0) + (transformViewport * vec4(logxOffset, logyOffset, 0.5, 1.0));
}

