#version 330

in vec2 vertexTex;

out vec4 fragColor;

uniform sampler2D fboTexture;
uniform bool bEnableHDR;
uniform float HDRExposure;
uniform bool bEnableBloom;

const vec3 emission = vec3(1, 1, 1);

void main() {
    float gamma = 2.2;

    vec3 color = pow(texture(fboTexture, vertexTex).rgb, vec3(gamma));

    if (bEnableBloom) {
        if(length(color) > 3.0) {
            color += emission;        
        }
    }

    if (bEnableHDR) {
        color = vec3(1.0) - vec3(exp(-color * HDRExposure));
    }

    color = pow(color, vec3(1.0 / gamma));

    fragColor = vec4(color, 1.0);
}