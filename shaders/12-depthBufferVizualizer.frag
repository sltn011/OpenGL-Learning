#version 330

out vec4 fragColor;

float near = 0.1; 
float far  = 100.0;
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // [-1, 1] 
    return (2.0 * near * far) / (far + near - z * (far - near)); // Inverse formula of normalized z value
}

void main()
{             
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    fragColor = vec4(vec3(depth), 1.0);
}
