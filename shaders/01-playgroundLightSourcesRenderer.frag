#version 330 core
float gamma = 2.2;

uniform vec3 color;



out vec4 fragColor;



void main() {
	//vec3 gammaCorrectedColor = pow(color, vec3(1.0 / gamma));
	fragColor = vec4(color, 1.0);
}