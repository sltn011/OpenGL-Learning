#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

out vec3 vertexPos;
out vec3 texCoords;
out vec3 normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
    vertexPos = vec3(model * vec4(aPos, 1.0));
	vec3 aTex = vec3(model * vec4(aPos, 0.0));
    texCoords = vec3(aTex.x, -aTex.y, -aTex.z);
	normal = vec3(normalize(model * vec4(aNorm, 0.0)));
    gl_Position = projection * view * model * vec4(aPos, 1.0);	
}