#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aTangent;

out vec2 vertexTexCoord;

void main()
{
   gl_Position = vec4(aPos, 1.0);
   vertexTexCoord = aTex;
}