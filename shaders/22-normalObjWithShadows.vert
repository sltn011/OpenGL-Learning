#version 330

#define MAX_DIR_LIGHTS   4

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 dirLightProjView[MAX_DIR_LIGHTS];

uniform int numDirLights;

out VS_OUT {
	vec3 vertexPos;
	vec3 vertexNorm;
	vec2 vertexTex;
	vec4 vertexPosLightSpace[MAX_DIR_LIGHTS];
} vs_out;


void main() {
	vs_out.vertexPos = vec3(model * vec4(aPos, 1.0));
	vs_out.vertexNorm = normalize(mat3(model) * aNorm);
	vs_out.vertexTex = aTex;
	for (int i = 0; i < numDirLights; ++i) {
		vs_out.vertexPosLightSpace[i] = dirLightProjView[i] * vec4(vs_out.vertexPos, 1.0);
	}
	gl_Position = projection * view * vec4(vs_out.vertexPos, 1.0);
}