#version 330

in vec2 vertexTex;

out vec4 fragColor;

uniform sampler2D fboTexture;

uniform int postprocessMode;

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
        sampleTex[i] = vec3(texture(fboTexture, vertexTex.st + offsets[i])); // st - same as xy
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++) {
		col += sampleTex[i] * kernel[i];
	}
	return col;
}

void main() {
	vec4 orig = texture(fboTexture, vertexTex);
	float average = 0.0;

	switch(postprocessMode) {
	case 0: // Normal
		fragColor = orig;
		break;

	case 1: // Negative
		fragColor = vec4(vec3(1.0 - orig), 1.0);
		break;

	case 2: // Black and white
		average = (orig.r + orig.g + orig.b) / 3.0;
		fragColor = vec4(average, average, average, 1.0);
		break;

	case 3: // Black and white weight channels
		average = orig.r * 0.2126 + orig.g * 0.7152 + orig.b * 0.0722;
		fragColor = vec4(average, average, average, 1.0);
		break;

	case 4: // Sharpen
		float kernelSharpen[9] = float[](
			+0.0, -1.0, +0.0,
			-1.0, +5.0, -1.0,
			+0.0, -1.0, +0.0
		);
		fragColor = vec4(applyKernel(kernelSharpen), 1.0);
		break;

	case 5: // Blur
		float kernelBlur[9] = float[](
			+1.0, +2.0, +1.0,
			+2.0, +4.0, +2.0,
			+1.0, +2.0, +1.0
		);
		fragColor = vec4(applyKernel(kernelBlur) / 16.0, 1.0);
		break;

	case 6: // Edge Detection
		float kernelEdgeDetection[9] = float[](
			+1.0, +1.0, +1.0,
			+1.0, -8.0, +1.0,
			+1.0, +1.0, +1.0
		);
		fragColor = vec4(applyKernel(kernelEdgeDetection), 1.0);
		break;

	case 7: // Outline - pretty much the same as edge detection
		float kernelOutline[9] = float[](
			-1.0, -1.0, -1.0,
			-1.0, +8.0, -1.0,
			-1.0, -1.0, -1.0
		);
		fragColor = vec4(applyKernel(kernelOutline), 1.0);
		break;

	case 8: // Top sobels
		float kernelTopSobels[9] = float[](
			+1.0, +2.0, +1.0,
			+0.0, +0.0, +0.0,
			-1.0, -2.0, -1.0
		);
		fragColor = vec4(applyKernel(kernelTopSobels), 1.0);
		break;

	case 9: // Bottom sobels
		float kernelBottomSobels[9] = float[](
			-1.0, -2.0, -1.0,
			+0.0, +0.0, +0.0,
			+1.0, +2.0, +1.0
		);
		fragColor = vec4(applyKernel(kernelBottomSobels), 1.0);
		break;

	case 10: // Left sobels
		float kernelLeftSobels[9] = float[](
			+1.0, +0.0, -1.0,
			+2.0, +0.0, -2.0,
			+1.0, +0.0, -1.0
		);
		fragColor = vec4(applyKernel(kernelLeftSobels), 1.0);
		break;

	case 11: // Right sobels
		float kernelRightSobels[9] = float[](
			-1.0, +0.0, +1.0,
			-2.0, +0.0, +2.0,
			-1.0, +0.0, +1.0
		);
		fragColor = vec4(applyKernel(kernelRightSobels), 1.0);
		break;

	default:
		fragColor = orig;
		break;
	}
}