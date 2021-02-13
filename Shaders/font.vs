#version 310 es
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2, tex>

out vec2 TexCoords;

uniform mat4 transformViewport;

void main() {
	gl_Position = transformViewport * vec4(vertex.xy, 0.5, 1.0);
	TexCoords = vertex.zw;
}
