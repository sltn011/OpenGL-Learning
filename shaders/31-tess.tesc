#version 410 core

layout(vertices=4) out;

in vec2 vertexTexCoord[];

out vec2 tescTexCoord[];

uniform int TessLevelOuter[4];
uniform int TessLevelInner[2];

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    
	tescTexCoord[gl_InvocationID] = vertexTexCoord[gl_InvocationID];

	if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = TessLevelOuter[0];
        gl_TessLevelOuter[1] = TessLevelOuter[1];
        gl_TessLevelOuter[2] = TessLevelOuter[2];
        gl_TessLevelOuter[3] = TessLevelOuter[3];
        gl_TessLevelInner[0] = TessLevelInner[0];
        gl_TessLevelInner[1] = TessLevelInner[1];
    }
}