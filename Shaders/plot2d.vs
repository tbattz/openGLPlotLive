#version 310 es
layout (location = 0) in vec2 coord2d;

uniform mat4 transformViewport;

void main(void) {
	gl_Position = transformViewport * vec4(coord2d.x, coord2d.y, 0.5, 1);
}