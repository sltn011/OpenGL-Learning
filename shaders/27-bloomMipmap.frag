#version 330

in vec2 vertexTex;

out vec4 fragColor;

uniform sampler2D fboTexture;
uniform vec3 thresHold;


void main() {
	vec3 color = texture(fboTexture, vertexTex).rgb - thresHold;
	color.r = max(color.r, 0.0);
	color.g = max(color.g, 0.0);
	color.b = max(color.b, 0.0);
    fragColor = vec4(color, 1.0);
}