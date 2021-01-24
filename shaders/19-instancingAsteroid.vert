#version 330
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vertexTex;

void main() {
	vertexTex = aTex;
	gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
}