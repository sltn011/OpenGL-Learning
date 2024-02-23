#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    texCoords = vec3(aPos.xyz);
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}