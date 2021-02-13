#version 310 es

precision highp float;

uniform vec4 inColor;

out vec4 color;

void main(void) {
	color = inColor;
}
