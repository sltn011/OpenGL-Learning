#version 330 core
out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube environmentMap;

void main() {    
    vec3 envColor = texture(environmentMap, texCoords).rgb;
    
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
  
    fragColor = vec4(envColor, 1.0);
}