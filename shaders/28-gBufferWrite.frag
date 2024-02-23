#version 330

#define MAX_DIR_LIGHTS   4
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS  4
#define MAX_DIFFUSE_TEXTURES  1
#define MAX_SPECULAR_TEXTURES 1
#define MAX_NORMAL_TEXTURES   1
#define MAX_DEPTH_TEXTURES   1
float gamma = 2.2;

float HeightScale = 0.03f;
int DepthLayers = 30;
int ReliefParallaxIterations = 30;



struct Material {
	sampler2D textureDiffuse[MAX_DIFFUSE_TEXTURES];
	int numDiffuseTextures;
	sampler2D textureSpecular[MAX_SPECULAR_TEXTURES];
	int numSpecularTextures;
	sampler2D textureNormal[MAX_NORMAL_TEXTURES];
	int numNormalTextures;
	sampler2D textureDepth[MAX_DEPTH_TEXTURES];
	int numDepthTextures;

	vec3 colorAmbient;
	vec3 colorDiffuse;
	vec3 colorSpecular;

	float specularExponent;
};



in VS_OUT {
	vec3 vertexPos;
	vec3 vertexNorm;
	vec2 vertexTex;
	mat3 TBN;
	vec3 vertexNormSS;
} fs_in;



layout(location = 0) out vec3 gWorldPosition;
layout(location = 1) out float gDepth;
layout(location = 2) out vec4 gAlbedoSpecular;
layout(location = 3) out vec3 gNormal;



uniform Material material;
uniform vec3 viewerPos;



vec3 calculateNormalVector(vec2 texCoord);

vec3 SteepParallaxMapping(vec3 viewDirTBN, vec2 texCoord);



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

	vec3 viewDir = normalize(fs_in.vertexPos - viewerPos);

	vec3 Parallax = SteepParallaxMapping(transpose(fs_in.TBN) * -viewDir, fs_in.vertexTex);
	vec2 texCoord = Parallax.xy * whenEq(material.numDepthTextures, 1) + fs_in.vertexTex * whenNe(material.numDepthTextures, 1);

	gWorldPosition.rgb = fs_in.vertexPos;
	
	gDepth = gl_FragCoord.z;

	gAlbedoSpecular.rgb = texture(material.textureDiffuse[0], texCoord).rgb;
	gAlbedoSpecular.a = texture(material.textureSpecular[0], texCoord).r;

	gNormal = normalize(fs_in.vertexNorm);
}



vec3 calculateNormalVector(vec2 texCoord) {
	vec3 surfaceNormal = normalize(fs_in.vertexNorm);
	vec3 sampledNormal = (texture(material.textureNormal[0], texCoord)).rgb;
	sampledNormal = sampledNormal * 2.0 - 1.0;
	sampledNormal = normalize(fs_in.TBN * sampledNormal);

	float useSampled = whenEq(material.numNormalTextures, 1.0);

	return normalize(useSampled * sampledNormal + (1.0 - useSampled) * surfaceNormal);
}

vec3 SteepParallaxMapping(vec3 viewDirTBN, vec2 texCoord) {

	float deltaLayerDepth = 1.0 / DepthLayers;
	float currentLayerDepth = 0.0;

	vec2 maxP = viewDirTBN.xy * HeightScale;
	maxP /= viewDirTBN.z;
	vec2 deltaTexCoords = maxP / DepthLayers;

	vec2 currentTexCoords = texCoord;
	float currentDepthValue = texture(material.textureDepth[0], currentTexCoords).r;

	while (currentLayerDepth < currentDepthValue) {
		currentTexCoords -= deltaTexCoords;
		currentDepthValue = texture(material.textureDepth[0], currentTexCoords).r;
		currentLayerDepth += deltaLayerDepth;
	}

	vec2 resultCoord = currentTexCoords;

	deltaTexCoords /= 2.0;
	deltaLayerDepth /= 2.0;

	currentTexCoords += deltaTexCoords;
	currentLayerDepth -= deltaLayerDepth;

	int currentIteration = ReliefParallaxIterations;
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

	return vec3(resultCoord, currentDepthValue);
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