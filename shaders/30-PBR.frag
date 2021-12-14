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
uniform float Roughness;
uniform float Metallic;

uniform vec3 viewerPos;
uniform int numPointLights;
uniform PointLight pointLight[MAX_POINT_LIGHTS];



float attenuation(PointLight Light, vec3 FragPos) {
	float LightFragDist = length(Light.position - FragPos);
	return 1.0 / (LightFragDist * LightFragDist);
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
vec3 FresnelSchlick(vec3 H, vec3 V, vec3 F0) {
	float HdotV = clamp(dot(H, V), 0.0, 1.0);
	return F0 + (1.0 - F0) * pow(clamp(1.0 - HdotV, 0.0, 1.0), 5.0);
}






void main() {
	vec3 WorldPosition = fs_in.vertexPos;

	vec3 V = normalize(viewerPos - WorldPosition);
	vec3 N = normalize(fs_in.vertexNorm);

	vec3 F0 = vec3(0.04); // Reflectance at normal incidence (0.04 for dielectrics)
	F0 = mix(F0, AlbedoColor, Metallic);

	vec3 LOut = vec3(0.0); // Reflectance equation
	for (int i = 0; i < numPointLights; ++i) {
		PointLight Light = pointLight[i];
		vec3 L = normalize(Light.position - WorldPosition);
		vec3 H = normalize(L + V);
		vec3 Radiance = Light.color * attenuation(Light, WorldPosition);


		// Cook-Torrance BRDF
		float NDF = TrowbridgeReitzGGX(N, H, Roughness);
		float G = SmithGGX(N, V, L, Roughness);
		vec3 F = FresnelSchlick(H, V, F0);

		vec3 Numerator = NDF * G * F;
		float Denominator = 4 * clamp(dot(V, N), 0.0, 1.0) * clamp(dot(L, N), 0.0, 1.0) + 0.0001; // Prevent division by zero
		vec3 Specular = Numerator / Denominator;

		vec3 Ks = F;
		vec3 Kd = 1.0 - F;
		Kd *= 1.0 - Metallic; // Pure metals don't have diffuse

		float NdotL = clamp(dot(N, L), 0.0, 1.0);

		LOut += (Kd * AlbedoColor / PI + Specular) * Radiance * NdotL;
	}

	vec3 Ambient = vec3(0.03) * AlbedoColor;

	vec3 Color = LOut + Ambient;
	
	// HDR
	Color = Color / (Color + 1.0);

	// Gamma correction
	Color = pow(Color, vec3(1.0 / Gamma));

	FragColor = vec4(Color, 1.0);
}