#version 310 es
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;

out vec3 fColor;

uniform mat4 transformViewport;

void main() {
    float logxOffset = log(aOffset.x) / log(10.0);
    float logyOffset = log(aOffset.y) / log(10.0);
    gl_Position = vec4(aPos, 0.0, 0.0) + (transformViewport * vec4(logxOffset, logyOffset, 0.5, 1.0));
}

