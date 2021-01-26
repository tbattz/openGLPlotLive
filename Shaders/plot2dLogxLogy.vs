#version 330 core
layout (location = 0) in vec2 coord2d;

uniform mat4 transformViewport;

void main(void) {
    float logx = log(coord2d.x) / log(10.0);
    float logy = log(coord2d.y) / log(10.0);
	gl_Position = transformViewport * vec4(logx, logy, 0.5, 1);
}