#version 330

in VS_OUT {
	vec3 vertexPos;
	vec3 vertexNorm;
	vec2 vertexTex;
	mat3 TBN;
} fs_in;

uniform vec3 viewerPos;
uniform float heightScale;
uniform bool bEnableParallaxMapping;
uniform bool bDivide;
uniform int DepthLayers;
uniform bool bEnableParallaxOcclusion;
uniform bool bEnableReliefParallax;
uniform int reliefParallaxIterations;
uniform bool bEnableSelfShadowing;

out vec4 fragColor;

float gamma = 2.2;

struct Material {
	sampler2D textureDiffuse[1];
	sampler2D textureSpecular[1];
	sampler2D textureNormal[1];
	sampler2D textureDepth[1];

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

float attenuationCoefficient(PointLight light, vec3 vertexPos);

vec3 ambientComponent(Material material, vec3 lightColor);
vec3 diffuseComponent(Material material, DirectionalLight light, vec3 normal);
vec3 diffuseComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal);
vec3 specularComponent(Material material, DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 specularComponent(Material material, PointLight light, vec3 vertexPos, vec3 normal, vec3 viewDir);

vec3 ParallaxMapping(vec3 viewDirTBN, vec2 texCoord);
vec3 SteepParallaxMapping(vec3 viewDirTBN, vec2 texCoord);
float ParallaxSelfShadowing(vec2 texCoord, vec3 tangengLightDir, float depth);

void main() {
	vec3 viewDir = normalize(fs_in.vertexPos - viewerPos);

	vec2 texCoord = fs_in.vertexTex;
	float shadowing = 1.0;

	if (bEnableParallaxMapping) {
		// Parallax Mapping
		vec3 viewDirTBN = transpose(fs_in.TBN) * -viewDir;
		vec3 tangentLightDir = transpose(fs_in.TBN) * (pointLight[0].position - fs_in.vertexPos);
		if (DepthLayers == 1) {
			vec3 Parallax = ParallaxMapping(viewDirTBN, fs_in.vertexTex);
			texCoord = Parallax.xy;
			if (bEnableSelfShadowing) {
				shadowing = ParallaxSelfShadowing(fs_in.vertexTex, tangentLightDir, Parallax.z);
			}
		}
		else {
			vec3 Parallax = SteepParallaxMapping(viewDirTBN, fs_in.vertexTex);
			texCoord = Parallax.xy;
			if (bEnableSelfShadowing) {
				shadowing = ParallaxSelfShadowing(fs_in.vertexTex, tangentLightDir, Parallax.z);
			}
		}

		if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
			discard; 

		//=====
	}

	float alpha = vec4(texture(material.textureDiffuse[0], texCoord)).a;
	
	if (alpha < 0.1) {
		discard;
	}

	vec3 diffuseCol = pow(vec3(texture(material.textureDiffuse[0], texCoord)), vec3(gamma));
	vec3 specularCol = vec3(texture(material.textureSpecular[0], texCoord));

	vec3 ambient  = vec3(0.0, 0.0, 0.0);
	vec3 diffuse  = vec3(0.0, 0.0, 0.0);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	// NORMAL MAPPING
	vec3 norm = vec3(texture(material.textureNormal[0], texCoord));
	norm = norm * 2.0 - 1.0;
	norm = normalize(fs_in.TBN * norm);
	//=====

	float attenuation = attenuationCoefficient(pointLight[0], fs_in.vertexPos);

	vec3 amb  = ambientComponent(material, pointLight[0].color);
	vec3 diff = attenuation * diffuseComponent(material, pointLight[0], fs_in.vertexPos, norm);
	vec3 spec = attenuation * specularComponent(material, pointLight[0], fs_in.vertexPos, norm, viewDir);
	ambient += amb;
	diffuse += diff;
	specular += spec * (diff == 0.0 ? 0 : 1);

	ambient *= diffuseCol;
	diffuse *= diffuseCol;
	specular *= specularCol;

	vec3 result = (ambient + diffuse + specular) * shadowing;

	result = pow(result, vec3(1.0 / gamma));
	fragColor = vec4(result, alpha);
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

vec3 ParallaxMapping(vec3 viewDirTBN, vec2 texCoord) {
	float height = texture(material.textureDepth[0], texCoord).r;
	vec2 xyView = viewDirTBN.xy;
	if (bDivide) {
		xyView /= viewDirTBN.z;
	}
	vec2 p = xyView * heightScale * height;
	return vec3(texCoord - p, height);
}

vec3 SteepParallaxMapping(vec3 viewDirTBN, vec2 texCoord) {

	float deltaLayerDepth = 1.0 / DepthLayers;
	float currentLayerDepth = 0.0;

	vec2 maxP = viewDirTBN.xy * heightScale;
	if (bDivide) {
		maxP /= viewDirTBN.z;
	}
	vec2 deltaTexCoords = maxP / DepthLayers;

	vec2 currentTexCoords = texCoord;
	float currentDepthValue = texture(material.textureDepth[0], currentTexCoords).r;

	while (currentLayerDepth < currentDepthValue) {
		currentTexCoords -= deltaTexCoords;
		currentDepthValue = texture(material.textureDepth[0], currentTexCoords).r;
		currentLayerDepth += deltaLayerDepth;
	}

	vec2 resultCoord = currentTexCoords;

	if (bEnableParallaxOcclusion) {
		vec2 beforeTexCoord = currentTexCoords + deltaTexCoords;

		float beforeDepth = texture(material.textureDepth[0], beforeTexCoord).r - currentLayerDepth + deltaLayerDepth;
		float afterDepth = currentDepthValue - currentLayerDepth;

		float alpha = afterDepth / (afterDepth - beforeDepth);
		resultCoord = beforeTexCoord * alpha + currentTexCoords * (1.0 - alpha);
	}
	else if (bEnableReliefParallax) {
		deltaTexCoords /= 2.0;
		deltaLayerDepth /= 2.0;

		currentTexCoords += deltaTexCoords;
		currentLayerDepth -= deltaLayerDepth;

		int currentIteration = reliefParallaxIterations;
		while (currentIteration > 0) {
			currentDepthValue = texture(material.textureDepth[0], currentTexCoords).r;
			deltaTexCoords /= 2.0;
			deltaLayerDepth /= 2.0;
			if (currentDepthValue > currentLayerDepth) {
				currentTexCoords -= deltaTexCoords;
				currentLayerDepth += deltaLayerDepth;
			}
			else {
				currentTexCoords += deltaTexCoords;
				currentLayerDepth -= deltaLayerDepth;
			}
			--currentIteration;
		}

		resultCoord = currentTexCoords;
	}

	return vec3(resultCoord, currentDepthValue);
}

float ParallaxSelfShadowing(vec2 texCoord, vec3 tangengLightDir, float depth) {

	float ShadowMultiplier = 0;
	
	if (dot(vec3(0.0, 0.0, 1.0), tangengLightDir) > 0.0) {
		float deltaDepth = depth / DepthLayers;
		vec2 deltaTexCoord = heightScale * tangengLightDir.xy / DepthLayers;
		if (bDivide) {
			deltaTexCoord /= tangengLightDir.z;
		}

		float currentLayerDepth = depth - deltaDepth;
		vec2 currentTexCoord = texCoord + deltaTexCoord;
		float currentDepthValue = texture(material.textureDepth[0], currentTexCoord).r;

		int samplesUnderSurface = 0;
		float stepIndex = 1.0;

		while (currentLayerDepth > 0) {
			if (currentDepthValue < currentLayerDepth) {
				++samplesUnderSurface;
				float currentShadowMultiplier = (currentLayerDepth - currentDepthValue) * (1.0 - stepIndex / DepthLayers);
				ShadowMultiplier = max(ShadowMultiplier, currentShadowMultiplier);
			}
			++stepIndex;
			currentLayerDepth -= deltaDepth;
			currentTexCoord += deltaTexCoord;
			currentDepthValue = texture(material.textureDepth[0], currentTexCoord).r;
		}

		ShadowMultiplier = samplesUnderSurface < 1 ? 1.0 : 1.0 - ShadowMultiplier;

	}
	else {
		ShadowMultiplier = 1.0;
	}

	return ShadowMultiplier;
}
