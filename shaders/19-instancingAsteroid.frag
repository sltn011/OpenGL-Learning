#version 330

in vec2 vertexTex;

out vec4 fragColor;

struct Material {
	sampler2D textureDiffuse1;
};

uniform Material material;

void main() {
	fragColor = vec4(texture(material.textureDiffuse1, vertexTex));
}