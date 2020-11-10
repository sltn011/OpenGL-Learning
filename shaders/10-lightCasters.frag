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

struct Light {
	vec3 color;
	vec3 pos;
	vec3 direction;

	float attConst;
	float attLinear;
	float attQuad;

	float cutOff;
	float cutOffOuter;

	int mode;
};

uniform Material material;
uniform Light light;

void main() {
	vec3 lightDir;
	vec3 ambient = vec3(1.0, 1.0, 1.0);
	vec3 diffuse = vec3(1.0, 1.0, 1.0);
	vec3 specular = vec3(1.0, 1.0, 1.0);

	if (light.mode == 0) {
		lightDir = normalize(vertPos - light.pos);
	}
	else if (light.mode == 1) {
		// can also set light vec as vec4
		// and when w == 0 do this calculation instead
		lightDir = normalize(light.direction);
	}
	else if (light.mode == 2) {
		lightDir = normalize(vertPos - light.pos);
		float dist = length(vertPos - light.pos);
		float attenuation = 1.0 / (light.attConst + light.attLinear * dist + light.attQuad * pow(dist, 2));
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
	}
	else if (light.mode == 3) {
		lightDir = normalize(vertPos - viewerPos);
		float theta = dot(normalize(viewerDir), lightDir);
		float epsilon = light.cutOff - light.cutOffOuter;
		float intensity = clamp((theta - light.cutOffOuter)/epsilon, 0.0, 1.0);
		if (theta > light.cutOffOuter) { // Area is lit
			// Attenuation
			float dist = length(vertPos - viewerPos);
			float attenuation = 1.0 / (light.attConst + light.attLinear * dist + light.attQuad * pow(dist, 2));
			diffuse *= attenuation * intensity;
			specular *= attenuation * intensity;
		}
		else {
			// Area is dark
			diffuse = vec3(0.0, 0.0, 0.0);
			specular = vec3(0.0, 0.0, 0.0);
		}
	}

	vec3 diffuseCol = vec3(texture(material.diffuseTex, vertTex));
	vec3 specularCol = vec3(texture(material.specularTex, vertTex));

	ambient *= diffuseCol * material.ambient * light.color;

	vec3 norm = normalize(vertNorm);
	float diff = -min(0.0, dot(lightDir, norm));
	diffuse *= diffuseCol * diff * material.diffuse * light.color;
	
	vec3 reflectedLight = normalize(reflect(lightDir, norm));
	vec3 viewDir = normalize(vertPos - viewerPos);
	float spec = pow(-min(0.0, dot(viewDir, reflectedLight)), 32);
	specular *= spec * material.specular * specularCol * light.color;

	fragColor = vec4(ambient + diffuse + specular, 1.0);
}