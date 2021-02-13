#version 310 es
layout (location = 0) in vec2 coord2d;

uniform mat4 transformViewport;

void main(void) {
    float logy = log(coord2d.y) / log(10.0);
	gl_Position = transformViewport * vec4(coord2d.x, logy, 0.5, 1);
}