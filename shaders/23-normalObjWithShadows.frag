#version 330

#define MAX_DIR_LIGHTS   4
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS  4
#define MAX_DIFFUSE_TEXTURES  1
#define MAX_SPECULAR_TEXTURES 1
#define MAX_NORMAL_TEXTURES   1
#define MAX_HEIGHT_TEXTURES   1

in VS_OUT {
	vec3 vertexPos;
	vec3 vertexNorm;
	vec2 vertexTex;
} fs_in;

uniform vec3 viewerPos;

out vec4 fragColor;

float gamma = 2.2;

struct Material {
	sampler2D textureDiffuse[MAX_DIFFUSE_TEXTURES];
	int numDiffuseTextures;
	sampler2D textureSpecular[MAX_SPECULAR_TEXTURES];
	int numSpecularTextures;
	sampler2D textureNormal[MAX_NORMAL_TEXTURES];
	int numNormalTextures;
	sampler2D textureHeight[MAX_HEIGHT_TEXTURES];
	int numHeightTextures;

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

uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;

uniform samplerCube pointLightShadowMap[MAX_POINT_LIGHTS];
uniform float pointLightShadowMapFarPlane[MAX_POINT_LIGHTS];

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

float calculateShadow(PointLight light, samplerCube map, float mapFarPlane, vec3 fragPos, float bias) {
	float shadow = 0.0;
	float numSamples = 4;
	float offset = 0.005;
	float delta = ((2 * offset) / numSamples);
	vec3 fragToLight = fragPos - light.position;
	fragToLight.yz *= -1;
	for (float x = -offset; x < offset; x += delta) {
		for (float y = -offset; y < offset; y += delta) {
			for (float z = -offset; z < offset; z += delta) {
				float closestDepth = texture(map, normalize(fragToLight + vec3(x, y, z))).r * mapFarPlane;
				float currentDepth = length(fragToLight);
				shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
			}
		}
	}
	return shadow / float(pow(numSamples, 3));
}

float calculateShadows(vec3 fragPos) {
	float shadows = 0.0;
	for (int i = 0; i < numPointLights; ++i) {
		shadows += calculateShadow(pointLight[i], pointLightShadowMap[i], pointLightShadowMapFarPlane[i], fragPos, 0.005); 
	}
	return shadows / float(numPointLights);
}

void main() {
	vec3 norm = normalize(fs_in.vertexNorm);
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

	for (int i = 0; i < numDirLights; ++i) {
		vec3 amb  = ambientComponent(material, directionalLight[i].color);
		vec3 diff = diffuseComponent(material, directionalLight[i], norm);
		vec3 spec = specularComponent(material, directionalLight[i], norm, viewDir);
		ambient += amb;
		diffuse += diff;
		specular += spec * (diff == 0.0 ? 0 : 1);
	}
	for (int i = 0; i < numPointLights; ++i) {
		vec3 amb  = ambientComponent(material, pointLight[i].color);
		vec3 diff = diffuseComponent(material, pointLight[i], fs_in.vertexPos, norm);
		vec3 spec = specularComponent(material, pointLight[i], fs_in.vertexPos, norm, viewDir);
		ambient += amb;
		diffuse += diff;
		specular += spec * (diff == 0.0 ? 0 : 1);
	}
	for (int i = 0; i < numSpotLights; ++i) {
		vec3 amb  = ambientComponent(material, spotLight[i].color);
		vec3 diff = diffuseComponent(material, spotLight[i], fs_in.vertexPos, norm);
		vec3 spec = specularComponent(material, spotLight[i], fs_in.vertexPos, norm, viewDir);
		ambient += amb;
		diffuse += diff;
		specular += spec * (diff == 0.0 ? 0 : 1);
	}

	ambient *= diffuseCol;
	diffuse *= diffuseCol;
	specular *= specularCol;

	float shadows = calculateShadows(fs_in.vertexPos); 
	vec3 result = (ambient + (1.0 - shadows) * (diffuse + specular));

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

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 vertexPos) {
	vec3 diffuseCol = pow(vec3(texture(material.textureDiffuse[0], fs_in.vertexTex)), vec3(gamma));
	vec3 specularCol = vec3(texture(material.textureSpecular[0], fs_in.vertexTex));

	float attenuation = attenuationCoefficient(light, vertexPos);

	vec3 ambient = ambientComponent(material, light.color) * diffuseCol;
	vec3 diffuse = diffuseComponent(material, light, vertexPos, normal) * diffuseCol;
	vec3 specular = specularComponent(material, light, vertexPos, normal, viewDir) * specularCol;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 vertexPos) {
	vec3 lightDir = normalize(vertexPos - light.position);

	vec3 diffuseCol = pow(vec3(texture(material.textureDiffuse[0], fs_in.vertexTex)), vec3(gamma));
	vec3 specularCol = vec3(texture(material.textureSpecular[0], fs_in.vertexTex));

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
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float dotProduct = dot(halfwayDir, normal);
	float spec = pow(-min(0.0, dotProduct), material.specularExponent);
	return spec * material.colorSpecular * light.color;
}

vec3 specularComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(vertexPos - light.position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float dotProduct = dot(halfwayDir, normal);
	float spec = pow(-min(0.0, dotProduct), material.specularExponent);
	return spec * material.colorSpecular * light.color;
}

vec3 specularComponent(Material material, SpotLight light, vec3 vertexPos, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(vertexPos - light.position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float dotProduct = dot(halfwayDir, normal);
	float spec = pow(-min(0.0, dotProduct), material.specularExponent);
	return spec * material.colorSpecular * light.color;
}