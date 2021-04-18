#version 330

uniform vec3 objectColor;

out vec4 fragColor;

void main() {
	fragColor = vec4(objectColor, 1.0f);
}