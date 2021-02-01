#version 330

in vec2 vertexTex;

out vec4 fragColor;

float near = 0.1; 
float far  = 100.0;

uniform sampler2D depthMap;
  
float linearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // [-1, 1] 
    return (2.0 * near * far) / (far + near - z * (far - near)); // Inverse formula of normalized z value
}

void main() {
	float depth = texture(depthMap, vertexTex).r;
	fragColor = vec4(vec3(depth), 1.0);
}