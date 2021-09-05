#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
	vec3 vertPos;
	vec3 vertNorm;
	vec2 texCoords;
} vs_out;

void main()
{
	vs_out.vertPos = aPos;
	vs_out.vertNorm = aNormal;
	vs_out.texCoords = aTexCoords;
}