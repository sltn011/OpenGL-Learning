#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertPos;
out vec3 vertNorm;
out vec2 vertTex;

void main() {
	vertPos = vec3(model * vec4(aPos, 1.0));
	vertNorm = mat3(model) * aNorm;
	//vertNorm = mat3(inverse(transpose(model))) * aNorm;
	vertTex = aTex;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}