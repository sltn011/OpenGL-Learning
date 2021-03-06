#version 330
#define MAX_DIR_LIGHTS   4
#define MAX_SPOT_LIGHTS  4



layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aTex;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int numDirLights;
uniform mat4 dirLightProjView[MAX_DIR_LIGHTS];
uniform int numSpotLights;
uniform mat4 spotLightProjView[MAX_SPOT_LIGHTS];



out VS_OUT {
	vec3 vertexPos;
	vec3 vertexNorm;
	vec3 vertexTex;
	vec4 vertexPosDirLightSpace[MAX_DIR_LIGHTS];
	vec4 vertexPosSpotLightSpace[MAX_SPOT_LIGHTS];
} vs_out;



void main() {
	vs_out.vertexPos = vec3(model * vec4(aPos, 1.0));
	vs_out.vertexNorm = normalize(mat3(model) * aNorm);
	vs_out.vertexTex = vec3(aTex.x, -aTex.y, -aTex.z);
	for (int i = 0; i < numDirLights; ++i) {
		vs_out.vertexPosDirLightSpace[i] = dirLightProjView[i] * vec4(vs_out.vertexPos, 1.0);
	}
	for (int i = 0; i < numSpotLights; ++i) {
		vs_out.vertexPosSpotLightSpace[i] = spotLightProjView[i] * vec4(vs_out.vertexPos, 1.0);
	}
	gl_Position = projection * view * vec4(vs_out.vertexPos, 1.0);
}