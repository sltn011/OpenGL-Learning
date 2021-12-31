#version 330
#define MAX_POINT_LIGHTS 4

float Gamma = 2.2;
const float PI = 3.14159265359;

struct PointLight {
	vec3 color;
	vec3 position;

	float attenuationConst;
	float attenuationLinear;
	float attenuationQuadratic;
};

in VS_OUT {
	vec3 vertexPos;
	vec3 vertexNorm;
	vec2 vertexTex;
	mat3 TBN;
} fs_in;

out vec4 FragColor;


uniform vec3 AlbedoColor;
uniform float RoughnessValue;
uniform float MetallicValue;

uniform bool bIsTextured;
uniform sampler2D AlbedoTexture;
uniform sampler2D NormalTexture;
uniform sampler2D RoughnessTexture;
uniform sampler2D MetallicTexture;
uniform sampler2D AOTexture;

uniform bool bUseIBL;
uniform samplerCube IrradianceCubemap;
uniform samplerCube PreFilteredCubemap;
uniform sampler2D   BRDFLookUpTable; 


uniform vec3 viewerPos;
uniform int numPointLights;
uniform PointLight pointLight[MAX_POINT_LIGHTS];



float attenuation(PointLight Light, vec3 FragPos) {
	float LightFragDist = length(Light.position - FragPos);
	return 1.0 / (LightFragDist * LightFragDist);
}



vec3 GetAlbedo() {
	return int(bIsTextured) * pow(texture(AlbedoTexture, fs_in.vertexTex).rgb, vec3(Gamma)) + (1 - int(bIsTextured)) * AlbedoColor; 
}

vec3 GetNormal() {
	vec3 SampledNorm = texture(NormalTexture, fs_in.vertexTex).rgb;
	SampledNorm = fs_in.TBN * normalize(SampledNorm * 2.0 - 1.0);
	return normalize(int(bIsTextured) * SampledNorm + (1 - int(bIsTextured)) * fs_in.vertexNorm);
}

float GetRoughness() {
	return int(bIsTextured) * texture(RoughnessTexture, fs_in.vertexTex).r + (1 - int(bIsTextured)) * RoughnessValue; 
}

float GetMetallic() {
	return int(bIsTextured) * texture(MetallicTexture, fs_in.vertexTex).r + (1 - int(bIsTextured)) * MetallicValue; 
}

float GetAO() {
	return int(bIsTextured) * texture(AOTexture, fs_in.vertexTex).r + (1 - int(bIsTextured)) * 1.0; 
}



//=====Cook-Torrance BRDF==================================
// D
float TrowbridgeReitzGGX(vec3 N, vec3 H, float Roughness) {
	float a = Roughness;
	float aSqr = a * a;
	float NdotH = clamp(dot(N, H), 0.0, 1.0);

	float Numerator = aSqr;
	float Denominator = (NdotH * NdotH * (aSqr - 1.0) + 1);
	Denominator = PI * Denominator * Denominator;

	return Numerator / Denominator;
}

// G sub
float SchlickGGX(float Dot, float Roughness) {
	float R = Roughness + 1.0;
	float K = (R * R) / 8.0;

	float Numerator = Dot;
	float Denominator = Dot * (1 - K) + K;

	return Numerator / Denominator;
}

// G
float SmithGGX(vec3 N, vec3 V, vec3 L, float Roughness) {
	float NdotV = clamp(dot(N, V), 0.0, 1.0);
	float NdotL = clamp(dot(N, L), 0.0, 1.0);

	float SGGX1 = SchlickGGX(NdotV, Roughness);
	float SGGX2 = SchlickGGX(NdotL, Roughness);
	return SGGX1 * SGGX2;
}

// F
vec3 FresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  





void main() {

	// Choose apropriate values for textured and untextured sphere
	vec3 Albedo = GetAlbedo();
	vec3 Normal = GetNormal();
	float Roughness = GetRoughness();
	float Metallic = GetMetallic();
	float AO = GetAO();

	vec3 WorldPosition = fs_in.vertexPos;

	vec3 V = normalize(viewerPos - WorldPosition);
	vec3 N = normalize(Normal);
	vec3 R = reflect(-V, N);

	vec3 F0 = vec3(0.04); // Reflectance at normal incidence (0.04 for dielectrics)
	F0 = mix(F0, Albedo, Metallic);

	vec3 LOut = vec3(0.0); // Reflectance equation
	for (int i = 0; i < numPointLights; ++i) {
		PointLight Light = pointLight[i];
		vec3 L = normalize(Light.position - WorldPosition);
		vec3 H = normalize(L + V);
		vec3 Radiance = Light.color * attenuation(Light, WorldPosition);


		// Cook-Torrance BRDF
		float NDF = TrowbridgeReitzGGX(N, H, Roughness);
		float G = SmithGGX(N, V, L, Roughness);
		vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

		vec3 Numerator = NDF * G * F;
		float Denominator = 4 * clamp(dot(V, N), 0.0, 1.0) * clamp(dot(L, N), 0.0, 1.0) + 0.0001; // Prevent division by zero
		vec3 Specular = Numerator / Denominator;

		vec3 Ks = F;
		vec3 Kd = 1.0 - F;
		Kd *= 1.0 - Metallic; // Pure metals don't have diffuse

		float NdotL = clamp(dot(N, L), 0.0, 1.0);

		LOut += (Kd * Albedo / PI + Specular) * Radiance * NdotL;
	}

	vec3 Ambient = vec3(0.0);
	if (bUseIBL) {
		vec3 F = FresnelSchlickRoughness(clamp(dot(N, V), 0.0, 1.0), F0, Roughness);
		vec3 Ks = F;
		vec3 Kd = 1.0 - Ks;
		Kd *= 1.0 - Metallic;

		vec3 Irradiance = texture(IrradianceCubemap, N).rgb;
		vec3 Diffuse = Irradiance * Albedo;

		const float MaxReflectionLOD = 4.0f;
		vec3 PreFilteredColor = textureLod(PreFilteredCubemap, R, Roughness * MaxReflectionLOD).rgb;
		vec2 BRDF = texture(BRDFLookUpTable, vec2(clamp(dot(N, V), 0.0, 1.0)), Roughness).xy;
		vec3 Specular = PreFilteredColor * (F * BRDF.x + BRDF.y);

		Ambient = (Kd * Diffuse + Specular) * AO;
	}
	else {
		Ambient = vec3(0.03) * Albedo * AO;
	}

	vec3 Color = Ambient + LOut;
	
	// HDR
	Color = Color / (Color + 1.0);

	// Gamma correction
	Color = pow(Color, vec3(1.0 / Gamma));

	FragColor = vec4(Color, 1.0);
}