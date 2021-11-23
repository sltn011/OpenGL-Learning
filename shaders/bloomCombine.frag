#version 330

in vec2 vertexTex;

out vec4 fragColor;

uniform sampler2D fboTexture1;
uniform sampler2D fboTexture2;

void main() {
	const float gamma = 2.2;
    vec3 color1 = pow(texture(fboTexture1, vertexTex).rgb, vec3(gamma));
    vec3 color2 = pow(texture(fboTexture2, vertexTex).rgb, vec3(gamma));
	vec3 result = pow(color1 + color2, vec3(1.0 / gamma));
    fragColor = vec4(result, 1.0);
}