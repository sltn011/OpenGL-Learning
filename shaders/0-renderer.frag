#version 330 core
out vec4 FragColor;

in vec3 vertPos;
in vec3 vertNorm;
in vec2 texCoords;

struct Material {
	vec3 colorAmbient;
	vec3 colorDiffuse;
	vec3 colorSpecular;
	
	float specularExponent;

	sampler2D textureDiffuse1;

	sampler2D textureSpecular1;

	sampler2D textureNormal1;

	sampler2D textureHeight1;
};

uniform Material material;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewerPos;

void main()
{
	vec3 texColor = vec3(texture(material.textureDiffuse1, texCoords));
	vec3 specColor = vec3(texture(material.textureSpecular1, texCoords));

    // Ambient
	vec3 ambient = lightColor * material.colorAmbient * texColor;

	// Diffuse
	vec3 norm = normalize(vertNorm);
	vec3 lightDir = normalize(vertPos - lightPos);
	float diff = -min(0.0, dot(norm, lightDir));
	vec3 diffuse = lightColor * (diff * material.colorDiffuse) * texColor;

	// Specular
	vec3 viewDir = normalize(vertPos - viewerPos);
	vec3 reflectedLight = reflect(lightDir, norm);
	float spec = pow(-min(0.0, dot(viewDir, reflectedLight)), material.specularExponent);
	vec3 specular = lightColor * (spec * material.colorSpecular) * texColor * specColor;

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
}