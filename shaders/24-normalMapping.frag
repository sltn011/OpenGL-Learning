#version 330

in VS_OUT {
	vec3 vertexPos;
	vec3 vertexNorm;
	vec2 vertexTex;
	mat3 TBN;
} fs_in;

uniform vec3 viewerPos;

uniform bool PerFragmentNormals;

out vec4 fragColor;

float gamma = 2.2;

struct Material {
	sampler2D textureDiffuse[1];
	sampler2D textureSpecular[1];
	sampler2D textureNormal[1];

	vec3 colorAmbient;
	vec3 colorDiffuse;
	vec3 colorSpecular;

	float specularExponent;
};

struct DirectionalLight {
	vec3 color;
	vec3 direction;
};

uniform Material material;

uniform DirectionalLight directionalLight[1];

vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir);


vec3 ambientComponent(Material material, vec3 lightColor);
vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal);
vec3 specularComponent(Material material, DirectionalLight light, vec3 normal, vec3 viewDir);

void main() {
	vec3 viewDir = normalize(fs_in.vertexPos - viewerPos);

	float alpha = vec4(texture(material.textureDiffuse[0], fs_in.vertexTex)).a;
	
	if (alpha < 0.1) {
		discard;
	}

	vec3 diffuseCol = pow(vec3(texture(material.textureDiffuse[0], fs_in.vertexTex)), vec3(gamma));
	vec3 specularCol = vec3(texture(material.textureSpecular[0], fs_in.vertexTex));

	vec3 ambient  = vec3(0.0, 0.0, 0.0);
	vec3 diffuse  = vec3(0.0, 0.0, 0.0);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	// NORMAL MAPPING
	vec3 norm = vec3(texture(material.textureNormal[0], fs_in.vertexTex));
	norm = norm * 2.0 - 1.0;
	norm = normalize(fs_in.TBN * norm);
	norm = PerFragmentNormals ? norm : normalize(fs_in.vertexNorm);
	//=====

	vec3 amb  = ambientComponent(material, directionalLight[0].color);
	vec3 diff = diffuseComponent(material, directionalLight[0], norm);
	vec3 spec = specularComponent(material, directionalLight[0], norm, viewDir);
	ambient += amb;
	diffuse += diff;
	specular += spec * (diff == 0.0 ? 0 : 1);

	ambient *= diffuseCol;
	diffuse *= diffuseCol;
	specular *= specularCol;

	vec3 result = ambient + diffuse + specular;

	result = pow(result, vec3(1.0 / gamma));
	fragColor = vec4(result, alpha);
}

vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 diffuseCol = pow(vec3(texture(material.textureDiffuse[0], fs_in.vertexTex)), vec3(gamma));
	vec3 specularCol = vec3(texture(material.textureSpecular[0], fs_in.vertexTex));

	vec3 ambient =  ambientComponent(material, light.color) * diffuseCol;
	vec3 diffuse = diffuseComponent(material, light, normal) * diffuseCol;
	vec3 specular = specularComponent(material, light, normal, viewDir) * specularCol;

	return (ambient + diffuse + specular);
}

vec3 ambientComponent(Material material, vec3 lightColor) {
	return material.colorAmbient * lightColor;
}

vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal) {
	vec3 lightDir = normalize(light.direction);
	float diff = -min(0.0, dot(lightDir, normal));
	return diff * material.colorDiffuse * light.color;
}

vec3 specularComponent(Material material, DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.direction);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float dotProduct = dot(halfwayDir, normal);
	float spec = pow(-min(0.0, dotProduct), material.specularExponent);
	return spec * material.colorSpecular * light.color;
}