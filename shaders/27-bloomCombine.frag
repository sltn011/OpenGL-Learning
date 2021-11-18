#version 330

in vec2 vertexTex;

out vec4 fragColor;

uniform sampler2D fboTexture;
uniform sampler2D resultTexture;

void main() {
    vec3 color = texture(fboTexture, vertexTex).rgb;
    vec3 resultColor = texture(resultTexture, vertexTex).rgb;
    fragColor = vec4(color + resultColor, 1.0);
}