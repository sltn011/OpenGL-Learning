#version 330 core
out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube skyboxSampler;

void main() {    
    fragColor = texture(skyboxSampler, texCoords);
}