#version 330

#define MAX_DIR_LIGHTS   4
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS  4
#define MAX_DIFFUSE_TEXTURES  1
#define MAX_SPECULAR_TEXTURES 1
#define MAX_NORMAL_TEXTURES   1
#define MAX_HEIGHT_TEXTURES   1
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



// COMPARISON TOOLS
vec4 whenEq(vec4 x, vec4 y); // ==
vec4 whenNe(vec4 x, vec4 y); // !=
vec4 whenGt(vec4 x, vec4 y); // >
vec4 whenLt(vec4 x, vec4 y); // <
vec4 whenGe(vec4 x, vec4 y); // >=
vec4 whenLe(vec4 x, vec4 y); // <=
vec4 and(vec4 x, vec4 y);
vec4 or(vec4 x, vec4 y);

float whenEq(float x, float y); // ==
float whenNe(float x, float y); // !=
float whenGt(float x, float y); // >
float whenLt(float x, float y); // <
float whenGe(float x, float y); // >=
float whenLe(float x, float y); // <=
float and(float x, float y);
float or(float x, float y);



void main() {
	vec3 norm = normalize(fs_in.vertexNorm);
	vec3 viewDir = normalize(fs_in.vertexPos - viewerPos);

	float alpha = vec4(texture(material.textureDiffuse[0], fs_in.vertexTex)).a;
	alpha *= whenNe(vec4(material.numDiffuseTextures), vec4(0)).a;
	
	vec3 diffuseCol = pow(vec3(texture(material.textureDiffuse[0], fs_in.vertexTex)), vec3(gamma));
	vec3 specularCol = vec3(texture(material.textureSpecular[0], fs_in.vertexTex));

	vec3 ambient  = vec3(0.0);
	vec3 diffuse  = vec3(0.0);
	vec3 specular = vec3(0.0);

	for (int i = 0; i < numDirLights; ++i) {
		DirectionalLight light = directionalLight[i];
		float shadow = calculateDirectShadow(i, norm);
		vec3 amb  = ambientComponent(material, light.color); // not affected by shadow
		light.color *= (1.0 - shadow);
		vec3 diff = diffuseComponent(material, light, norm);
		vec3 spec = specularComponent(material, light, norm, viewDir);
		ambient += amb;
		diffuse += diff;
		specular += spec * whenNe(vec4(diffuse, 1.0), vec4(0.0)).x;
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
		specular += spec * attenuation * whenNe(vec4(diffuse, 1.0), vec4(0.0)).x;
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
		specular += spec * attenuation * litCoeff * whenNe(vec4(diffuse, 1.0), vec4(0.0)).x;
	}

	ambient *= diffuseCol * whenNe(material.numDiffuseTextures, 0);
	diffuse *= diffuseCol * whenNe(material.numDiffuseTextures, 0);
	specular *= specularCol * whenNe(material.numSpecularTextures, 0);

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
	
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(dirLightShadowMap[lightIndex], 0);
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			float mapDepth = texture(dirLightShadowMap[lightIndex], projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += 1.0 * whenGt(currentDepth - bias, mapDepth);
		}
	}
	return (shadow / 9.0) * whenLe(currentDepth, 1.0);
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
				shadow += 1.0 * whenGt(currentDepth - bias, closestDepth);
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
	
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(spotLightShadowMap[lightIndex], 0);
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			float mapDepth = texture(spotLightShadowMap[lightIndex], projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += 1.0 * whenGt(currentDepth - bias, mapDepth);
		}
	}
	return (shadow / 9.0) * whenLe(currentDepth, 1.0);
}



// COMPARISON TOOLS
vec4 whenEq(vec4 x, vec4 y) {
	return 1.0 - abs(sign(x - y));
}

vec4 whenNe(vec4 x, vec4 y){
	return abs(sign(x - y));
}

vec4 whenGt(vec4 x, vec4 y){
	return max(sign(x - y), 0.0);
}

vec4 whenLt(vec4 x, vec4 y){
	return max(sign(y - x), 0.0);
}

vec4 whenGe(vec4 x, vec4 y){
	return 1.0 - whenLt(x, y);
}

vec4 whenLe(vec4 x, vec4 y){
	return 1.0 - whenGt(x, y);
}

vec4 and(vec4 x, vec4 y){
	return x * y;
}

vec4 or(vec4 x, vec4 y){
	return min(x + y, 1.0);
}


float whenEq(float x, float y) {
	return 1.0 - abs(sign(x - y));
}

float whenNe(float x, float y){
	return abs(sign(x - y));
}

float whenGt(float x, float y){
	return max(sign(x - y), 0.0);
}

float whenLt(float x, float y){
	return max(sign(y - x), 0.0);
}

float whenGe(float x, float y){
	return 1.0 - whenLt(x, y);
}

float whenLe(float x, float y){
	return 1.0 - whenGt(x, y);
}

float and(float x, float y){
	return x * y;
}

float or(float x, float y){
	return min(x + y, 1.0);
}