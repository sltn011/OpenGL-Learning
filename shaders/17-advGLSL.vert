#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;

layout(std140) uniform Matrices {
	mat4 view;
	mat4 projection;
};

uniform int showcaseMode;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	if (showcaseMode == 0) {
		gl_PointSize = gl_Position.z * 5;
	}
}