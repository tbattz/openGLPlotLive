#version 330 core
layout (location = 0) in vec2 coord2d;

uniform mat4 transformViewport;
uniform float logYBase;

void main(void) {
    float logy = log(coord2d.y) / log(logYBase);
	gl_Position = transformViewport * vec4(coord2d.x, logy, 0.5, 1);
}