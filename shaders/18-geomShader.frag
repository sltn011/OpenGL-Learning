#version 330 core

in GS_OUT {
	vec4 color;
} gs_out;

out vec4 FragColor;

void main() {
    FragColor = gs_out.color;   
} 