#version 330 core

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lampColor;

void main() {
	fragColor = vec4(objectColor * lampColor, 1.0);
}