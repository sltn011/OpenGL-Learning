#version 330 core



in vec3 texCoords;



out vec4 fragColor;



uniform samplerCube skyboxSampler;




void main() {
    fragColor = texture(skyboxSampler, texCoords);
}