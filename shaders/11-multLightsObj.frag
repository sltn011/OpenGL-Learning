#version 330

in vec3 vertexPos;
in vec3 vertexNorm;
in vec2 vertexTex;

uniform vec3 viewerPos;

out vec4 fragColor;

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

struct PointLight {
	vec3 color;
	vec3 position;

	float attenuationConst;
	float attenuationLinear;
	float attenuationQuadratic;
};

struct SpotLight {
	vec3 color;
	vec3 position;
	vec3 direction;

	float cutOffCos;
	float cutOffOuterCos;

	float attenuationConst;
	float attenuationLinear;
	float attenuationQuadratic;
};

uniform Material material;

#define MAX_DIR_LIGHTS   4
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS  4

uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;

uniform DirectionalLight directionalLight[MAX_DIR_LIGHTS];
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];

vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 vertexPos);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 vertexPos);

float attenuationCoefficient(PointLight light, vec3 vertexPos);
float attenuationCoefficient(SpotLight light, vec3 vertexPos);

vec3 ambientComponent(Material material, vec3 lightColor);
vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal);
vec3 diffuseComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal);
vec3 diffuseComponent(Material material, SpotLight light, vec3 vertexPos, vec3 normal);
vec3 specularComponent(Material material, DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 specularComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal, vec3 viewDir);
vec3 specularComponent(Material material, SpotLight light, vec3 vertexPos, vec3 normal, vec3 viewDir);

void main() {
	vec3 norm = normalize(vertexNorm);
	vec3 viewDir = normalize(vertexPos - viewerPos);

	vec3 res = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < numDirLights; ++i) {
		res += calculateDirectLight(directionalLight[i], norm, viewDir);
	}
	for (int i = 0; i < numPointLights; ++i) {
		res += calculatePointLight(pointLight[i], norm, viewDir, vertexPos);
	}
	for (int i = 0; i < numSpotLights; ++i) {
		res += calculateSpotLight(spotLight[i], norm, viewDir, vertexPos);
	}

	fragColor = vec4(res, 1.0);
}

vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 diffuseCol = vec3(texture(material.textureDiffuse1, vertexTex));
	vec3 specularCol = vec3(texture(material.textureSpecular1, vertexTex));

	vec3 ambient =  ambientComponent(material, light.color) * diffuseCol;
	vec3 diffuse = diffuseComponent(material, light, normal) * diffuseCol;
	vec3 specular = specularComponent(material, light, normal, viewDir) * specularCol;

	return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 vertexPos) {
	vec3 diffuseCol = vec3(texture(material.textureDiffuse1, vertexTex));
	vec3 specularCol = vec3(texture(material.textureSpecular1, vertexTex));

	float attenuation = attenuationCoefficient(light, vertexPos);

	vec3 ambient = ambientComponent(material, light.color) * diffuseCol;
	vec3 diffuse = diffuseComponent(material, light, vertexPos, normal) * diffuseCol;
	vec3 specular = specularComponent(material, light, vertexPos, normal, viewDir) * specularCol;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 vertexPos) {
	vec3 lightDir = normalize(vertexPos - light.position);

	vec3 diffuseCol = vec3(texture(material.textureDiffuse1, vertexTex));
	vec3 specularCol = vec3(texture(material.textureSpecular1, vertexTex));

	float lightRayAngleCos = dot(normalize(light.direction), lightDir);
	float fadingCoefficient = light.cutOffCos - light.cutOffOuterCos;
	float intensity = clamp((lightRayAngleCos - light.cutOffOuterCos)/fadingCoefficient, 0.0, 1.0);

	float attenuation = attenuationCoefficient(light, vertexPos);

    vec3 diffuse = diffuseComponent(material, light, vertexPos, normal) * diffuseCol;
    vec3 specular = specularComponent(material, light, vertexPos, normal, viewDir) * specularCol;
    
    return (diffuse + specular) * attenuation * intensity;
}

float attenuationCoefficient(PointLight light, vec3 vertexPos) {
	float dist = length(vertexPos - light.position);
	float attenuation = 1.0 / (light.attenuationConst + light.attenuationLinear * dist + light.attenuationQuadratic * pow(dist, 2));
	return attenuation;
}

float attenuationCoefficient(SpotLight light, vec3 vertexPos) {
	float dist = length(vertexPos - light.position);
	float attenuation = 1.0 / (light.attenuationConst + light.attenuationLinear * dist + light.attenuationQuadratic * pow(dist, 2));
	return attenuation;
}

vec3 ambientComponent(Material material, vec3 lightColor) {
	return material.colorAmbient * lightColor;
}

vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal) {
	vec3 lightDir = normalize(light.direction);
	float diff = -min(0.0, dot(lightDir, normal));
	return diff * material.colorDiffuse * light.color;
}

vec3 diffuseComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal) {
	vec3 lightDir = normalize(vertexPos - light.position);
	float diff = -min(0.0, dot(lightDir, normal));
	return diff * material.colorDiffuse * light.color;
}

vec3 diffuseComponent(Material material, SpotLight light, vec3 vertexPos, vec3 normal) {
	vec3 lightDir = normalize(vertexPos - light.position);
	float diff = -min(0.0, dot(lightDir, normal));
	return diff * material.colorDiffuse * light.color;
}

vec3 specularComponent(Material material, DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.direction);
	vec3 reflectedLight = normalize(reflect(lightDir, normal));
	float spec = pow(-min(0.0, dot(viewDir, reflectedLight)), material.specularExponent);
	return spec * material.colorSpecular * light.color;
}

vec3 specularComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(vertexPos - light.position);
	vec3 reflectedLight = normalize(reflect(lightDir, normal));
	float spec = pow(-min(0.0, dot(viewDir, reflectedLight)), material.specularExponent);
	return spec * material.colorSpecular * light.color;
}

vec3 specularComponent(Material material, SpotLight light, vec3 vertexPos, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(vertexPos - light.position);
	vec3 reflectedLight = normalize(reflect(lightDir, normal));
	float spec = pow(-min(0.0, dot(viewDir, reflectedLight)), material.specularExponent);
	return spec * material.colorSpecular * light.color;
}