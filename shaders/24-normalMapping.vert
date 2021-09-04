#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
	vec3 vertexPos;
	vec3 vertexNorm;
	vec2 vertexTex;
	mat3 TBN;
} vs_out;


void main() {
	vs_out.vertexPos = vec3(model * vec4(aPos, 1.0));
	vs_out.vertexNorm = normalize(vec3(model * vec4(aNorm, 0.0)));
	vs_out.vertexTex = aTex;

	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = vs_out.vertexNorm;
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	vs_out.TBN = mat3(T, B, N);
	
	//vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));

	gl_Position = projection * view * vec4(vs_out.vertexPos, 1.0);
}