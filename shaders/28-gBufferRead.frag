#version 330

#define MAX_DIR_LIGHTS     4
#define MAX_POINT_LIGHTS   4
#define MAX_SPOT_LIGHTS    4

const vec3 ambientCoeff = vec3(0.2, 0.2, 0.2);
const vec3 diffuseCoeff = vec3(0.5, 0.5, 0.5);
const vec3 specularCoeff = vec3(0.25, 0.25, 0.25);
const float specularExp = 51.2;


in vec2 vertexTex;

out vec4 fragColor;

uniform vec3 viewerPos;

uniform sampler2D gWorldPosition;
uniform sampler2D gDepth;
uniform sampler2D gAlbedoSpecular;
uniform sampler2D gNormal;



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



uniform int numDirLights;
uniform mat4 dirLightProjView[MAX_DIR_LIGHTS];
uniform int numSpotLights;
uniform mat4 spotLightProjView[MAX_SPOT_LIGHTS];


uniform int numPointLights;
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

vec3 ambientComponent(vec3 lightColor);
vec3 diffuseComponent(DirectionalLight light, vec3 normal);
vec3 diffuseComponent(PointLight light, vec3 vertexPos, vec3 normal);
vec3 diffuseComponent(SpotLight light, vec3 vertexPos, vec3 normal);
vec3 specularComponent(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 specularComponent(PointLight light, vec3 vertexPos, vec3 normal, vec3 viewDir);
vec3 specularComponent(SpotLight light, vec3 vertexPos, vec3 normal, vec3 viewDir);

float calculateDirectShadow(vec3 worldPos, int lightIndex, vec3 fragNormal);
float calculatePointShadow(vec3 worldPos, int lightIndex);
float calculateSpotShadow(vec3 worldPos, int lightIndex, vec3 fragNormal);



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
    float gamma = 2.2;

	vec3 worldPos = texture(gWorldPosition, vertexTex).rgb;
    vec3 color = pow(texture(gAlbedoSpecular, vertexTex).rgb, vec3(gamma));
	vec3 normal = normalize(texture(gNormal, vertexTex).rgb);
	
	vec3 viewDir = normalize(worldPos - viewerPos);

	vec3 ambient  = vec3(0.0);
	vec3 diffuse  = vec3(0.0);
	vec3 specular = vec3(0.0);

	for (int i = 0; i < numDirLights; ++i) {
		DirectionalLight light = directionalLight[i];
		float shadow = calculateDirectShadow(worldPos, i, normal);
		vec3 amb  = ambientComponent(light.color); // not affected by shadow
		light.color *= (1.0 - shadow);
		vec3 diff = diffuseComponent(light, normal);
		vec3 spec = specularComponent(light, normal, viewDir);
		ambient += amb;
		diffuse += diff;
		specular += spec * whenNe(vec4(diffuse, 1.0), vec4(0.0)).x;
	}
	for (int i = 0; i < numPointLights; ++i) {
		PointLight light = pointLight[i];
		float attenuation = attenuationCoefficient(light, worldPos);
		float shadow = calculatePointShadow(worldPos, i);
		vec3 amb  = ambientComponent(light.color); // not affected by shadow
		light.color *= (1.0 - shadow);
		vec3 diff = diffuseComponent(light, worldPos, normal);
		vec3 spec = specularComponent(light, worldPos, normal, viewDir);
		ambient += amb * attenuation;
		diffuse += diff * attenuation;
		specular += spec * attenuation * whenNe(vec4(diffuse, 1.0), vec4(0.0)).x;
	}
	for (int i = 0; i < numSpotLights; ++i) {
		SpotLight light = spotLight[i];
		float attenuation = attenuationCoefficient(light, worldPos);
		float litCoeff = spotLightLitArea(light, worldPos);
		float shadow = calculateSpotShadow(worldPos, i, normal);
		vec3 amb  = ambientComponent(light.color); // not affected by shadow
		light.color *= (1.0 - shadow);
		vec3 diff = diffuseComponent(light, worldPos, normal);
		vec3 spec = specularComponent(light, worldPos, normal, viewDir);
		ambient += amb * attenuation;
		diffuse += diff * attenuation * litCoeff;
		specular += spec * attenuation * litCoeff * whenNe(vec4(diffuse, 1.0), vec4(0.0)).x;
	}

	ambient *= color;
	diffuse *= color;
	specular *= color;

	vec3 result = ambient + diffuse + specular;

    result = pow(result, vec3(1.0 / gamma));

    fragColor = vec4(result, 1.0);
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

vec3 ambientComponent(vec3 lightColor) {
	return ambientCoeff * lightColor;
}

vec3 diffuseComponent(DirectionalLight light, vec3 normal) {
	vec3 lightDir = normalize(light.direction);
	float diff = -min(0.0, dot(lightDir, normal));
	return diff * diffuseCoeff * light.color;
}

vec3 diffuseComponent(PointLight light, vec3 vertexPos, vec3 normal) {
	vec3 lightDir = normalize(vertexPos - light.position);
	float diff = -min(0.0, dot(lightDir, normal));
	return diff * diffuseCoeff * light.color;
}

vec3 diffuseComponent(SpotLight light, vec3 vertexPos, vec3 normal) {
	vec3 lightDir = normalize(vertexPos - light.position);
	float diff = -min(0.0, dot(lightDir, normal));
	return diff * diffuseCoeff * light.color;
}

vec3 specularComponent(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.direction);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float dotProduct = dot(halfwayDir, normal);
	float spec = pow(-min(0.0, dotProduct), specularExp);
	return spec * specularCoeff * light.color;
}

vec3 specularComponent(PointLight light, vec3 vertexPos, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(vertexPos - light.position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float dotProduct = dot(halfwayDir, normal);
	float spec = pow(-min(0.0, dotProduct), specularExp);
	return spec * specularCoeff * light.color;
}

vec3 specularComponent(SpotLight light, vec3 vertexPos, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(vertexPos - light.position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float dotProduct = dot(halfwayDir, normal);
	float spec = pow(-min(0.0, dotProduct), specularExp);
	return spec * specularCoeff * light.color;
}

float calculateDirectShadow(vec3 worldPos, int lightIndex, vec3 fragNormal) {
	float bias = max(0.00025 * (1.0 - dot(fragNormal, normalize(directionalLight[lightIndex].direction))), 0.000025);
	vec4 vertexPosDirLightSpace = dirLightProjView[lightIndex] * vec4(worldPos, 1.0);
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

float calculatePointShadow(vec3 worldPos, int lightIndex) {
	float shadow = 0.0;
	float numSamples = 4;
	float offset = 0.005;
	float bias = 0.005;
	float delta = ((2 * offset) / numSamples);
	vec3 fragToLight = worldPos - pointLight[lightIndex].position;
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

float calculateSpotShadow(vec3 worldPos, int lightIndex, vec3 fragNormal) {
	float bias = max(0.00025 * (1.0 - dot(fragNormal, normalize(spotLight[lightIndex].direction))), 0.000025);
	vec4 vertexPosSpotLightSpace = spotLightProjView[lightIndex] * vec4(worldPos, 1.0);
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