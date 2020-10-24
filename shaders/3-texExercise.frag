#version 330 core

in vec3 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

uniform float mixing;

void main()
{
   vec2 flippedYFaceTex = vec2(-texCoord.x, texCoord.y);
   fragColor = mix(texture(myTexture1, texCoord), texture(myTexture2, flippedYFaceTex), texture(myTexture2, texCoord).a * mixing);
}