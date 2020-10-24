#version 330 core

out vec4 fragColor;

uniform vec3 lampColor;

void main() {
	fragColor = vec4(lampColor, 1.0);
}