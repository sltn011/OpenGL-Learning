#version 330 core
out vec4 fragColor;

in vec3 vertexPos;
in vec3 texCoords;
in vec3 normal;

uniform vec3 viewerPos;

uniform vec3 mirrorColor;
uniform samplerCube cubemapSampler;

void main() {    
    vec3 norm = normalize(normal);
	vec3 viewDir = normalize(vertexPos - viewerPos);
	vec3 reflected = reflect(viewDir, norm);
	reflected.yz *= -1;
	fragColor = vec4(texture(cubemapSampler, reflected).rgb * mirrorColor, 1.0);
	//fragColor = vec4(texture(cubemapSampler, texCoords).rgb, 1.0);
}