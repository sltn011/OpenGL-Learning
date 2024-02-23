#version 410 core

in vec2 UV;
uniform sampler2D heightMap;

out vec4 fragColor;

void main()
{
   fragColor = vec4(texture(heightMap, UV).xyz, 1);
}