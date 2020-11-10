#version 330 core

out vec4 fragColor;

#define NUM_POINT_LIGHTS 2
uniform int lampIndex;
uniform vec3 lampColor[NUM_POINT_LIGHTS];

void main() {
	int ind = clamp(lampIndex, 0, NUM_POINT_LIGHTS - 1);
	fragColor = vec4(lampColor[ind], 1.0);
}