#version 330

in vec3 vertPos;
in vec3 vertNorm;
in vec2 vertTex;

uniform vec3 viewerPos;
uniform vec3 viewerDir;

out vec4 fragColor;

struct Material {
	sampler2D diffuseTex;
	sampler2D specularTex;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirectionalLight {
	vec3 color;
	vec3 direction;
};

struct PointLight {
	vec3 color;
	vec3 pos;

	float attConst;
	float attLinear;
	float attQuad;
};

struct FlashLight {
	vec3 color;

	float cutOff;
	float cutOffOuter;

	float attConst;
	float attLinear;
	float attQuad;
};

uniform Material material;

#define NUM_POINT_LIGHTS 2

uniform DirectionalLight dirLight;
uniform PointLight pointLight[NUM_POINT_LIGHTS];
uniform FlashLight flashLight;

vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 vertPos);
vec3 calculateFlashLight(FlashLight light, vec3 normal, vec3 vertPos, vec3 viewDir, vec3 viewPos);

void main() {
	vec3 norm = normalize(vertNorm);
	vec3 viewDir = normalize(vertPos - viewerPos);

	vec3 res = calculateDirectLight(dirLight, norm, viewDir);

	for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
		res += calculatePointLight(pointLight[i], norm, viewDir, vertPos);
	}
	res += calculateFlashLight(flashLight, norm, vertPos, viewerDir, viewerPos);

	fragColor = vec4(res, 1.0);
}

vec3 calculateDirectLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.direction);

	vec3 diffuseCol = vec3(texture(material.diffuseTex, vertTex));
	vec3 specularCol = vec3(texture(material.specularTex, vertTex));

	vec3 ambient = material.ambient * diffuseCol * light.color;

	float diff = -min(0.0, dot(lightDir, normal));
	vec3 diffuse = material.diffuse * diffuseCol * light.color;

	vec3 reflectedLight = normalize(reflect(lightDir, normal));
	float spec = pow(-min(0.0, dot(viewDir, reflectedLight)), 32);
	vec3 specular = spec * material.specular * specularCol * light.color;

	return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 vertPos) {
	vec3 lightDir = normalize(vertPos - light.pos);

	vec3 diffuseCol = vec3(texture(material.diffuseTex, vertTex));
	vec3 specularCol = vec3(texture(material.specularTex, vertTex));

	float dist = length(vertPos - light.pos);
	float attenuation = 1.0 / (light.attConst + light.attLinear * dist + light.attQuad * pow(dist, 2));

	vec3 ambient = material.ambient * diffuseCol * light.color;

	float diff = -min(0.0, dot(lightDir, normal));
	vec3 diffuse = material.diffuse * diffuseCol * light.color;

	vec3 reflectedLight = normalize(reflect(lightDir, normal));
	float spec = pow(-min(0.0, dot(viewDir, reflectedLight)), 32);
	vec3 specular = spec * material.specular * specularCol * light.color;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateFlashLight(FlashLight light, vec3 normal, vec3 vertPos, vec3 viewerDir, vec3 viewerPos) {
	vec3 lightDir = normalize(vertPos - viewerPos);

	vec3 diffuseCol = vec3(texture(material.diffuseTex, vertTex));
	vec3 specularCol = vec3(texture(material.specularTex, vertTex));

	float theta = dot(normalize(viewerDir), lightDir);
	float epsilon = light.cutOff - light.cutOffOuter;
	float intensity = clamp((theta - light.cutOffOuter)/epsilon, 0.0, 1.0);
	if (theta > light.cutOffOuter) { // Area is lit
		// Attenuation
		float dist = length(vertPos - viewerPos);
		float attenuation = 1.0 / (light.attConst + light.attLinear * dist + light.attQuad * pow(dist, 2));
		
		float diff = -min(0.0, dot(lightDir, normal));
		vec3 diffuse = material.diffuse * diffuseCol * light.color;

		vec3 reflectedLight = normalize(reflect(lightDir, normal));
		float spec = pow(-min(0.0, dot(viewerDir, reflectedLight)), 32);
		vec3 specular = spec * material.specular * specularCol * light.color;

		return (diffuse + specular) * attenuation * intensity;
	}
	else {
		return vec3(0.0, 0.0, 0.0);
	}
}