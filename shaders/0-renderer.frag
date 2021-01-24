#version 330 core
out vec4 FragColor;

in GS_OUT {
	vec3 vertPos;
	vec3 vertNorm;
	vec2 texCoords;
} gs_out;

struct Material {
	sampler2D textureDiffuse1;
	sampler2D textureSpecular1;

	vec3 colorAmbient;
	vec3 colorDiffuse;
	vec3 colorSpecular;

	float specularExponent;
};

struct DirectionalLight {
	vec3 color;
	vec3 direction;
};

uniform vec3 viewerPos;

uniform Material material;

uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;

uniform DirectionalLight directionalLight;

vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir);

vec3 ambientComponent(Material material, vec3 lightColor);
vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal);
vec3 specularComponent(Material material, DirectionalLight light, vec3 normal, vec3 viewDir);

void main() {
	vec3 norm = normalize(gs_out.vertNorm);
	vec3 viewDir = normalize(gs_out.vertPos - viewerPos);

	vec3 res = vec3(0.0, 0.0, 0.0);
	float alpha = vec4(texture(material.textureDiffuse1, gs_out.texCoords)).a;
	
	if (alpha < 0.1) {
		discard;
	}

	res += calculateDirectLight(directionalLight, norm, viewDir);

	FragColor = vec4(res, alpha);
}

vec3 ambientComponent(Material material, vec3 lightColor) {
	return material.colorAmbient * lightColor;
}

vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 diffuseCol = vec3(texture(material.textureDiffuse1, gs_out.texCoords));
	vec3 specularCol = vec3(texture(material.textureSpecular1, gs_out.texCoords));

	vec3 ambient =  ambientComponent(material, light.color) * diffuseCol;
	vec3 diffuse = diffuseComponent(material, light, normal) * diffuseCol;
	vec3 specular = specularComponent(material, light, normal, viewDir) * specularCol;

	return (ambient + diffuse + specular);
}

vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal) {
	vec3 lightDir = normalize(light.direction);
	float diff = -min(0.0, dot(lightDir, normal));
	return diff * material.colorDiffuse * light.color;
}

vec3 specularComponent(Material material, DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.direction);
	vec3 reflectedLight = normalize(reflect(lightDir, normal));
	float spec = pow(-min(0.0, dot(viewDir, reflectedLight)), material.specularExponent);
	return spec * material.colorSpecular * light.color;
}