#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
	vec3 vertPos;
	vec3 vertNorm;
} vs_out;

void main()
{
	vs_out.vertPos = aPos;
	vs_out.vertNorm = aNormal;
}