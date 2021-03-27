#version 330
layout(points) in;
layout(line_strip, max_vertices = 2) out;

in vec3 pos[1];

uniform vec3 axis;

uniform mat4 view;
uniform mat4 projection;

void main() {
	
	gl_Position = projection * view * vec4(10 * axis + pos[0], 1.0);
	EmitVertex();

	gl_Position = projection * view * vec4(10 * -axis + pos[0], 1.0);
	EmitVertex();

	EndPrimitive();
}