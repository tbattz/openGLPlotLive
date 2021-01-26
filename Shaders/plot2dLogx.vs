#version 330 core
layout (location = 0) in vec2 coord2d;

uniform mat4 transformViewport;

void main(void) {
    float logx = log(coord2d.x) / log(10.0);
	gl_Position = transformViewport * vec4(logx, coord2d.y, 0.5, 1);
}