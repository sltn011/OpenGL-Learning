#version 330

in vec2 vertexTex;

out vec4 fragColor;

uniform sampler2D fboTexture;
uniform int imageWidth;

void main() {
    vec3 color = vec3(0.0);

    const int samples = 5;
    vec2 blurTexCoord[2 * samples + 1];

    float pixelSize = 1.0 / imageWidth;
	for (int i = -samples; i <= samples; ++i) {
		blurTexCoord[i + samples] = vertexTex + vec2(i * pixelSize, 0.0);
	}

    color += texture(fboTexture, blurTexCoord[0]).rgb * 0.0093;
    color += texture(fboTexture, blurTexCoord[1]).rgb * 0.028002;
    color += texture(fboTexture, blurTexCoord[2]).rgb * 0.065984;
    color += texture(fboTexture, blurTexCoord[3]).rgb * 0.121703;
    color += texture(fboTexture, blurTexCoord[4]).rgb * 0.175713;
    color += texture(fboTexture, blurTexCoord[5]).rgb * 0.198596;
    color += texture(fboTexture, blurTexCoord[6]).rgb * 0.175713;
    color += texture(fboTexture, blurTexCoord[7]).rgb * 0.121703;
    color += texture(fboTexture, blurTexCoord[8]).rgb * 0.065984;
    color += texture(fboTexture, blurTexCoord[9]).rgb * 0.028002;
    color += texture(fboTexture, blurTexCoord[10]).rgb * 0.0093;

    fragColor = vec4(color, 1.0);
}