#version 330 core

in vec2 texCoord;
in vec4 vertexColor;

out vec4 fragColor;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main()
{
   fragColor = vertexColor;
}