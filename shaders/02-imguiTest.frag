#version 330 core
float gamma = 2.2;

in vec3 vertNormal;



struct Material {
	vec3 colorAmbient;
	vec3 colorDiffuse;
};

struct DirectionalLight {
	vec3 color;
	vec3 direction;
};



uniform Material material;
uniform DirectionalLight directionalLight[1];
uniform vec3 objectColor;



vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 ambientComponent(Material material, vec3 lightColor);
vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal);
vec3 specularComponent(Material material, DirectionalLight light, vec3 normal, vec3 viewDir);



out vec4 FragColor;



void main() {
	vec3 norm = normalize(vertNormal);

	vec3 res = vec3(0.0, 0.0, 0.0);

	res += ambientComponent(material, directionalLight[0].color);
	res += diffuseComponent(material, directionalLight[0], norm);

	vec3 coloredFrag = pow(objectColor * res, vec3(1.0 / gamma));

	FragColor = vec4(coloredFrag, 1.0);
}

vec3 ambientComponent(Material material, vec3 lightColor) {
	return material.colorAmbient * lightColor;
}

vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal) {
	vec3 lightDir = normalize(light.direction);
	float diff = -min(0.0, dot(lightDir, normal));
	return diff * material.colorDiffuse * light.color;
}