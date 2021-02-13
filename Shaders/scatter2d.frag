#version 310 es

precision highp float;

out vec4 FragColor;

uniform vec4 inColor;

void main() {
    FragColor = inColor;
}

