#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexture;

out vec2 texCoord;

uniform mat4 transMatr;

void main()
{
   gl_Position = transMatr * vec4(aPos, 1.0);
   texCoord = aTexture;
}