#version 330

in vec2 vertexTex;

out float occlusion;

uniform sampler2D gWorldPosition;
uniform sampler2D gDepth;
uniform sampler2D gAlbedoSpecular;
uniform sampler2D gNormal;

void main() {
    vec3 color = texture(gAlbedoSpecular, vertexTex).rgb;

    occlusion = color.r;
}