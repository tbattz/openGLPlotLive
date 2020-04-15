#version 330 core

uniform vec4 colPos;
uniform vec4 colNeg;

in float yVals;
out vec4 color;

void main(void) {
	float ySign = sign(yVals);
	color = vec4(0.5f*(colPos[0]-colNeg[0])*ySign + 0.5f*(colPos[0]+colNeg[0]),
				 0.5f*(colPos[1]-colNeg[1])*ySign + 0.5f*(colPos[1]+colNeg[1]),
				 0.5f*(colPos[2]-colNeg[2])*ySign + 0.5f*(colPos[2]+colNeg[2]),
				 0.5f*(colPos[3]-colNeg[3])*ySign + 0.5f*(colPos[3]+colNeg[3]));
}