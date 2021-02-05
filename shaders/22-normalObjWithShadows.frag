#version 330

#define MAX_DIR_LIGHTS   4
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS  4

in VS_OUT {

	vec3 vertexPos;
	vec3 vertexNorm;
	vec2 vertexTex;
	vec4 vertexPosLightSpace[MAX_DIR_LIGHTS];

} fs_in;

uniform vec3 viewerPos;

out vec4 fragColor;

float gamma = 2.2;

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

	float cutOffAngle;
	float cutOffOuterAngle;

	float attenuationConst;
	float attenuationLinear;
	float attenuationQuadratic;
};

uniform Material material;

uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;

uniform sampler2D shadowMap[MAX_DIR_LIGHTS];

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

float calculateShadow(sampler2D map, vec4 vertexPosLightSpace, float bias) {
	vec3 projCoords = vertexPosLightSpace.xyz / vertexPosLightSpace.w; // perspective divide
	projCoords = projCoords * 0.5 + 0.5; // transform coords from [-1, 1] to [0, 1]
	float currentDepth = projCoords.z;
	if (currentDepth > 1.0) {
		return 0.0;
	}
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(map, 0);
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			float mapDepth = texture(map, projCoords.xy + vec2(x, y) * texelSize).r; // get depth from depth map
			shadow += currentDepth - bias > mapDepth ? 1.0 : 0.0;
		}
	}
	return shadow / 9.0;
}

float calculateShadows(vec3 fragNormal) {
	float shadows = 0.0;
	for (int i = 0; i < numDirLights; ++i) {
		float bias = max(0.00025 * (1.0 - dot(fragNormal, normalize(directionalLight[i].direction))), 0.000025);
		shadows += calculateShadow(shadowMap[i], fs_in.vertexPosLightSpace[i], bias);
	}
	return shadows / numDirLights;
}

void main() {
	vec3 norm = normalize(fs_in.vertexNorm);
	vec3 viewDir = normalize(fs_in.vertexPos - viewerPos);

	float alpha = vec4(texture(material.textureDiffuse1, fs_in.vertexTex)).a;
	
	if (alpha < 0.1) {
		discard;
	}

	vec3 diffuseCol = pow(vec3(texture(material.textureDiffuse1, fs_in.vertexTex)), vec3(gamma));
	vec3 specularCol = vec3(texture(material.textureSpecular1, fs_in.vertexTex));

	vec3 ambient  = vec3(0.0, 0.0, 0.0);
	vec3 diffuse  = vec3(0.0, 0.0, 0.0);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < numDirLights; ++i) {
		ambient += ambientComponent(material, directionalLight[i].color);
		diffuse += diffuseComponent(material, directionalLight[i], norm);
		specular += specularComponent(material, directionalLight[i], norm, viewDir);
	}
	for (int i = 0; i < numPointLights; ++i) {
		ambient += ambientComponent(material, pointLight[i].color);
		diffuse += diffuseComponent(material, pointLight[i], fs_in.vertexPos, norm);
		specular += specularComponent(material, pointLight[i], fs_in.vertexPos, norm, viewDir);
	}
	for (int i = 0; i < numSpotLights; ++i) {
		ambient += ambientComponent(material, spotLight[i].color);
		diffuse += diffuseComponent(material, spotLight[i], fs_in.vertexPos, norm);
		specular += specularComponent(material, spotLight[i], fs_in.vertexPos, norm, viewDir);
	}

	ambient *= diffuseCol;
	diffuse *= diffuseCol;
	specular *= specularCol;

	float shadows = calculateShadows(norm);
	vec3 result = (ambient + (1.0 - shadows) * (diffuse + specular));

	result = pow(result, vec3(1.0 / gamma));
	fragColor = vec4(result, alpha);
}

vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 diffuseCol = pow(vec3(texture(material.textureDiffuse1, fs_in.vertexTex)), vec3(gamma));
	vec3 specularCol = vec3(texture(material.textureSpecular1, fs_in.vertexTex));

	vec3 ambient =  ambientComponent(material, light.color) * diffuseCol;
	vec3 diffuse = diffuseComponent(material, light, normal) * diffuseCol;
	vec3 specular = specularComponent(material, light, normal, viewDir) * specularCol;

	return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 vertexPos) {
	vec3 diffuseCol = pow(vec3(texture(material.textureDiffuse1, fs_in.vertexTex)), vec3(gamma));
	vec3 specularCol = vec3(texture(material.textureSpecular1, fs_in.vertexTex));

	float attenuation = attenuationCoefficient(light, vertexPos);

	vec3 ambient = ambientComponent(material, light.color) * diffuseCol;
	vec3 diffuse = diffuseComponent(material, light, vertexPos, normal) * diffuseCol;
	vec3 specular = specularComponent(material, light, vertexPos, normal, viewDir) * specularCol;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 vertexPos) {
	float cutOffCos = cos(light.cutOffAngle);
	float cutOffOuterCos = cos(light.cutOffOuterAngle);

	vec3 lightDir = normalize(vertexPos - light.position);

	vec3 diffuseCol = pow(vec3(texture(material.textureDiffuse1, fs_in.vertexTex)), vec3(gamma));
	vec3 specularCol = vec3(texture(material.textureSpecular1, fs_in.vertexTex));

	float lightrayAngleCos = dot(normalize(light.direction), lightDir);
	float fadingCoefficient = cutOffCos - cutOffOuterCos;
	float intensity = clamp((lightrayAngleCos - cutOffOuterCos)/fadingCoefficient, 0.0, 1.0);

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