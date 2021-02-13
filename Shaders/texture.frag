#version 310 es

precision highp float;

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;


void main(void) {
	FragColor = texture(ourTexture, TexCoord);
}
