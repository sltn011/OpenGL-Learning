#version 330

in vec2 vertexTex;

out vec4 fragColor;

uniform sampler2D fboTexture;
uniform bool bEnableHDR;
uniform float HDRExposure;

void main() {
    float gamma = 2.2;

    vec3 hdrColor = pow(texture(fboTexture, vertexTex).rgb, vec3(gamma));

    if (bEnableHDR) {
        hdrColor = vec3(1.0) - vec3(exp(-hdrColor * HDRExposure));
    }

    hdrColor = pow(hdrColor, vec3(1.0 / gamma));

    fragColor = vec4(hdrColor, 1.0);
}