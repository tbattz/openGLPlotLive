#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2, tex>

out vec2 TexCoords;

uniform mat4 textProjection;

void main() {
	gl_Position = textProjection * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}