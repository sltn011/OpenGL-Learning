#version 330

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 vertexTex;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	vertexTex = aTex;
}