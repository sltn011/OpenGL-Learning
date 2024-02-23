#version 330

in VS_OUT {
	vec3 vertexPos;
	vec3 vertexNorm;
	vec2 vertexTex;
	mat3 TBN;
} fs_in;

uniform mat4 view;
uniform vec3 viewerPos;

uniform bool PerFragmentNormals;
uniform int UsedTexture;

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

struct PointLight {
	vec3 color;
	vec3 position;

	float attenuationConst;
	float attenuationLinear;
	float attenuationQuadratic;
};

uniform Material material;

uniform DirectionalLight directionalLight[1];
uniform PointLight pointLight[1];

vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir);

float attenuationCoefficient(PointLight light, vec3 vertexPos);

vec3 ambientComponent(Material material, vec3 lightColor);
vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal);
vec3 diffuseComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal);
vec3 specularComponent(Material material, DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 specularComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal, vec3 viewDir);

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

	vec3 n1 = vec3(texture(material.textureNormal[0], fs_in.vertexTex)) * 2.f - 1.f;
	vec3 n2 = vec3(texture(material.textureDiffuse[0], fs_in.vertexTex)) * 2.f - 1.f;

	vec3 n;

	if (!PerFragmentNormals)
		{
			n = normalize(n1 + n2);
		}
		else
		{
			n1 += vec3(0, 0, 1);
			n2 *= vec3(-1, -1, 1);
			n = n1*dot(n1, n2)/n1.z - n2; 
			n = normalize(n);
		}

	n = normalize(fs_in.TBN * n);
	n = (view * vec4(n, 1)).xyz;

	fragColor = vec4(n * 0.5f + 0.5f, alpha);
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



float attenuationCoefficient(PointLight light, vec3 vertexPos) {
	float dist = length(vertexPos - light.position);
	float attenuation = 1.0 / (light.attenuationConst + light.attenuationLinear * dist + light.attenuationQuadratic * pow(dist, 2));
	return attenuation;
}

vec3 diffuseComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal) {
	vec3 lightDir = normalize(vertexPos - light.position);
	float diff = -min(0.0, dot(lightDir, normal));
	return diff * material.colorDiffuse * light.color;
}

vec3 specularComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(vertexPos - light.position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float dotProduct = dot(halfwayDir, normal);
	float spec = pow(-min(0.0, dotProduct), material.specularExponent);
	return spec * material.colorSpecular * light.color;
}