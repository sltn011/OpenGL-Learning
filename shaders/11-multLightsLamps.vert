#version 330 core
layout (location = 0) in vec3 aPos;

#define NUM_POINT_LIGHTS 2
uniform mat4 model[NUM_POINT_LIGHTS];
uniform mat4 view;
uniform mat4 projection;

uniform int lampIndex;

void main() {
	int ind = clamp(lampIndex, 0, NUM_POINT_LIGHTS - 1);
	gl_Position = projection * view * model[ind] * vec4(aPos, 1.0);
}