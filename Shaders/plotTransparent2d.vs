#version 330 core
layout (location = 0) in vec2 coord2d;

uniform mat4 transformViewport;
uniform float z;

void main(void) {
	gl_Position = transformViewport * vec4(coord2d.x, coord2d.y, z, 1);
}