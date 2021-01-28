#version 330 core
layout (location = 0) in vec2 coord2d;

uniform mat4 transformViewport;
uniform uint logXBase;
uniform uint logYBase;

void main(void) {
    float logx = log(coord2d.x) / log(logXBase);
    float logy = log(coord2d.y) / log(logYBase);
	gl_Position = transformViewport * vec4(logx, logy, 0.5, 1);
}