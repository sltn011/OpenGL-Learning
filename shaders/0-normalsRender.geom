#version 330
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 vertPos;
	vec3 vertNorm;
} vs_out[];

uniform mat4 model;

layout(std140) uniform Matrices {
	mat4 view;
	mat4 projection;
};

const float ampl = 0.4;

void genLine(int index) {
	vec3 normalOffset = vs_out[index].vertNorm * ampl;

	// Base vertex
	gl_Position = projection * view * model * vec4(vs_out[index].vertPos, 1.0);
	EmitVertex();

	// Out vertex
	gl_Position = projection * view * model * vec4(vs_out[index].vertPos + normalOffset, 1.0);
	EmitVertex();
	EndPrimitive();
}

void main() {
	for(int i = 0; i < 3; ++i) {
		genLine(i);
	}
}