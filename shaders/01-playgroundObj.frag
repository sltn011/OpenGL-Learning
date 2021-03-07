#version 330

#define MAX_DIR_LIGHTS   4
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS  4
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

	float cutOffCos;
	float cutOffOuterCos;

	float attenuationConst;
	float attenuationLinear;
	float attenuationQuadratic;
};



in VS_OUT {
	vec3 vertexPos;
	vec3 vertexNorm;
	vec2 vertexTex;
	vec4 vertexPosDirLightSpace[MAX_DIR_LIGHTS];
	vec4 vertexPosSpotLightSpace[MAX_SPOT_LIGHTS];
} fs_in;



out vec4 fragColor;



uniform Material material;
uniform vec3 viewerPos;
uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;
uniform sampler2D dirLightShadowMap[MAX_DIR_LIGHTS];
uniform samplerCube pointLightShadowMap[MAX_POINT_LIGHTS];
uniform float pointLightShadowMapFarPlane[MAX_POINT_LIGHTS];
uniform sampler2D spotLightShadowMap[MAX_SPOT_LIGHTS];
uniform DirectionalLight directionalLight[MAX_DIR_LIGHTS];
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];



float attenuationCoefficient(PointLight light, vec3 vertexPos);
float attenuationCoefficient(SpotLight light, vec3 vertexPos);

float spotLightLitArea(SpotLight light, vec3 vertexPos);

vec3 ambientComponent(Material material, vec3 lightColor);
vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal);
vec3 diffuseComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal);
vec3 diffuseComponent(Material material, SpotLight light, vec3 vertexPos, vec3 normal);
vec3 specularComponent(Material material, DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 specularComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal, vec3 viewDir);
vec3 specularComponent(Material material, SpotLight light, vec3 vertexPos, vec3 normal, vec3 viewDir);

float calculateDirectShadow(int lightIndex, vec3 fragNormal);
float calculatePointShadow(int lightIndex, vec3 fragPos);
float calculateSpotShadow(int lightIndex, vec3 fragNormal);



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
		DirectionalLight light = directionalLight[i];
		float shadow = calculateDirectShadow(i, norm);
		vec3 amb  = ambientComponent(material, light.color); // not affected by shadow
		light.color *= (1.0 - shadow);
		vec3 diff = diffuseComponent(material, light, norm);
		vec3 spec = specularComponent(material, light, norm, viewDir);
		ambient += amb;
		diffuse += diff;
		specular += spec * (diff == 0.0 ? 0 : 1);
	}
	for (int i = 0; i < numPointLights; ++i) {
		PointLight light = pointLight[i];
		float attenuation = attenuationCoefficient(light, fs_in.vertexPos);
		float shadow = calculatePointShadow(i, fs_in.vertexPos);
		vec3 amb  = ambientComponent(material, light.color); // not affected by shadow
		light.color *= (1.0 - shadow);
		vec3 diff = diffuseComponent(material, light, fs_in.vertexPos, norm);
		vec3 spec = specularComponent(material, light, fs_in.vertexPos, norm, viewDir);
		ambient += amb * attenuation;
		diffuse += diff * attenuation;
		specular += spec * attenuation * (diff == 0.0 ? 0 : 1);
	}
	for (int i = 0; i < numSpotLights; ++i) {
		SpotLight light = spotLight[i];
		float attenuation = attenuationCoefficient(light, fs_in.vertexPos);
		float litCoeff = spotLightLitArea(light, fs_in.vertexPos);
		float shadow = calculateSpotShadow(i, norm);
		vec3 amb  = ambientComponent(material, light.color); // not affected by shadow
		light.color *= (1.0 - shadow);
		vec3 diff = diffuseComponent(material, light, fs_in.vertexPos, norm);
		vec3 spec = specularComponent(material, light, fs_in.vertexPos, norm, viewDir);
		ambient += amb * attenuation;
		diffuse += diff * attenuation * litCoeff;
		specular += spec * attenuation * litCoeff * (diff == 0.0 ? 0 : 1);
	}

	ambient *= diffuseCol;
	diffuse *= diffuseCol;
	specular *= specularCol;

	vec3 result = ambient + diffuse + specular;

	result = pow(result, vec3(1.0 / gamma));
	fragColor = vec4(result, alpha);
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

float spotLightLitArea(SpotLight light, vec3 vertexPos) {
	vec3 lightRayDir = normalize(vertexPos - light.position);
	float lightRayAngleCos = dot(light.direction, lightRayDir);
	float fadingCoefficient = light.cutOffCos - light.cutOffOuterCos;
	float intensity = clamp((lightRayAngleCos - light.cutOffOuterCos)/fadingCoefficient, 0.0, 1.0);
	return intensity; 
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

float calculateDirectShadow(int lightIndex, vec3 fragNormal) {
	float bias = max(0.00025 * (1.0 - dot(fragNormal, normalize(directionalLight[lightIndex].direction))), 0.000025);
	vec4 vertexPosDirLightSpace = fs_in.vertexPosDirLightSpace[lightIndex];
	vec3 projCoords = vertexPosDirLightSpace.xyz / vertexPosDirLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;
	if (currentDepth > 1.0) {
		return 0.0;
	}
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(dirLightShadowMap[lightIndex], 0);
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			float mapDepth = texture(dirLightShadowMap[lightIndex], projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > mapDepth ? 1.0 : 0.0;
		}
	}
	return shadow / 9.0;
}

float calculatePointShadow(int lightIndex, vec3 fragPos) {
	float shadow = 0.0;
	float numSamples = 4;
	float offset = 0.005;
	float bias = 0.005;
	float delta = ((2 * offset) / numSamples);
	vec3 fragToLight = fragPos - pointLight[lightIndex].position;
	fragToLight.yz *= -1;
	for (float x = -offset; x < offset; x += delta) {
		for (float y = -offset; y < offset; y += delta) {
			for (float z = -offset; z < offset; z += delta) {
				float closestDepth = texture(pointLightShadowMap[lightIndex], normalize(fragToLight + vec3(x, y, z))).r * pointLightShadowMapFarPlane[lightIndex];
				float currentDepth = length(fragToLight);
				shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
			}
		}
	}
	return shadow / float(pow(numSamples, 3));
}

float calculateSpotShadow(int lightIndex, vec3 fragNormal) {
	float bias = max(0.00025 * (1.0 - dot(fragNormal, normalize(spotLight[lightIndex].direction))), 0.000025);
	vec4 vertexPosSpotLightSpace = fs_in.vertexPosSpotLightSpace[lightIndex];
	vec3 projCoords = vertexPosSpotLightSpace.xyz / vertexPosSpotLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;
	if (currentDepth > 1.0) {
		return 0.0;
	}
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(spotLightShadowMap[lightIndex], 0);
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			float mapDepth = texture(spotLightShadowMap[lightIndex], projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > mapDepth ? 1.0 : 0.0;
		}
	}
	return shadow / 9.0;
}