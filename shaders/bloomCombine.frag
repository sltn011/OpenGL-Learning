#version 330

const float gamma = 2.2;

in vec2 vertexTex;

out vec4 fragColor;

uniform sampler2D fboTexture1;
uniform sampler2D fboTexture2;
uniform bool bLastCombine;

const float offset = 1.0 / 540.0;

const vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0,     offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0),    // center-left
        vec2( 0.0,     0.0),    // center-center
        vec2( offset,  0.0),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0,    -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
);

vec3 applyKernel(float kernel[9]) {
	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        vec3 color1 = pow(texture(fboTexture1, vertexTex + offsets[i]).rgb, vec3(gamma));
        vec3 color2 = pow(texture(fboTexture2, vertexTex + offsets[i] * (bLastCombine ? 0.0 : 1.0)).rgb, vec3(gamma));
        sampleTex[i] = pow(color1 + color2, vec3(1.0 / gamma));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++) {
		col += sampleTex[i] * kernel[i];
	}
	return col;
}

void main() {
    float kernelBlur[9] = float[](
			+1.0, +2.0, +1.0,
			+2.0, +4.0, +2.0,
			+1.0, +2.0, +1.0
		);
    vec3 color = applyKernel(kernelBlur) / 16.0;
    fragColor = vec4(color, 1.0);
}