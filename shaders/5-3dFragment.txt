#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main()
{
   fragColor = mix(texture(myTexture1, texCoord), texture(myTexture2, texCoord), texture(myTexture2, texCoord).a * 0.5);
}