#version 410 core

layout (quads, equal_spacing , ccw) in;

in vec2 tescTexCoord[];

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D heightMap;
uniform bool bEnableHeightMap;

vec4 bilinearInterpPosition()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 c00 = gl_in[0].gl_Position;      // 10 --- 11
    vec4 c01 = gl_in[1].gl_Position;      // |      |
    vec4 c10 = gl_in[2].gl_Position;      // |      |
    vec4 c11 = gl_in[3].gl_Position;      // 00 --- 01

    vec4 c0001 = (1.f - u) * c00 + u * c01;
    vec4 c1011 = (1.f - u) * c10 + u * c11;
    vec4 c = (1.f - v) * c0001 + v * c1011;

    return c;
}

vec2 bilinearInterpUV()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 c00 = tescTexCoord[0];      // 10 --- 11
    vec2 c01 = tescTexCoord[1];      // |      |
    vec2 c10 = tescTexCoord[2];      // |      |
    vec2 c11 = tescTexCoord[3];      // 00 --- 01

    vec2 c0001 = (1.f - u) * c00 + u * c01;
    vec2 c1011 = (1.f - u) * c10 + u * c11;
    vec2 c = (1.f - v) * c0001 + v * c1011;

    return c;
}

void main()
{
    vec4 pos = bilinearInterpPosition();
    vec2 texCoord = bilinearInterpUV();
    float offset = texture(heightMap, texCoord).r;
    pos.y += offset * float(bEnableHeightMap) * 5.f;
    gl_Position = projection * view * model * pos;
    vertexColor = vec3(offset, offset, offset);
}