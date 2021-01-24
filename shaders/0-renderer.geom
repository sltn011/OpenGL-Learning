#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 vertPos;
	vec3 vertNorm;
	vec2 texCoords;
} vs_out[];

out GS_OUT {
	vec3 vertPos;
	vec3 vertNorm;
	vec2 texCoords;
} gs_out;

uniform bool doExplode;
uniform float explodeTime;

uniform mat4 model;

layout(std140) uniform Matrices {
	mat4 view;
	mat4 projection;
};

const float ampl = 0.25;

vec3 faceNormal() {
	vec3 a = vec3(vs_out[0].vertPos - vs_out[1].vertPos);
	vec3 b = vec3(vs_out[2].vertPos - vs_out[1].vertPos);
	return -1 * normalize(cross(a, b));
}

vec3 explodeOffset(vec3 direction) {
	if (doExplode) {
		return direction * explodeTime * ampl;
	}
	else {
		return vec3(0.0, 0.0, 0.0);
	}
}

void main() {
	vec3 offset = explodeOffset(faceNormal());
	for(int i = 0; i < 3; ++i) {
		gs_out.vertPos = vec3(model * vec4(vs_out[i].vertPos + offset, 1.0));
		gs_out.vertNorm = normalize(mat3(model) * vs_out[i].vertNorm);
		gs_out.texCoords = vs_out[i].texCoords;
		gl_Position = projection * view * model * vec4(vs_out[i].vertPos + offset, 1.0);
		EmitVertex();
	}
	EndPrimitive();
}