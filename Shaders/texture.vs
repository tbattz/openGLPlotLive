#version 330 core
layout (location = 0) in vec2 coord2d;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 transformViewport;

out vec2 TexCoord;


void main(void) {
	gl_Position = transformViewport * vec4(coord2d.x, coord2d.y, 0.5, 1);
	TexCoord = aTexCoord;
}