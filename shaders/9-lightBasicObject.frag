#version 330 core

in vec3 vecNormal;
in vec3 fragPos;

out vec4 fragColor;

uniform int lightMode;

uniform float ambientStr;
uniform float specularStr;
uniform int shininess;

uniform vec3 objectColor;
uniform vec3 lampColor;

uniform vec3 lampPos;
uniform vec3 viewerPos;

void main() {
	
	if (lightMode == 0) { // Ambient

		vec3 ambient = lampColor * ambientStr;

		fragColor = vec4(ambient * objectColor, 1.0);
	}
	else if (lightMode == 1) { // Diffuse
		vec3 norm = normalize(vecNormal);
		vec3 lightDir = normalize(fragPos - lampPos);
		float diff = -1 * min(0.0, dot(norm, lightDir));

		vec3 diffuse = diff * lampColor;

		fragColor = vec4(diffuse * objectColor, 1.0);
	}
	else if (lightMode == 2) { // Specular
		vec3 norm = normalize(vecNormal);
		vec3 lightDir = normalize(fragPos - lampPos);
		vec3 viewDir = normalize(fragPos - viewerPos);
		vec3 reflectedLight = reflect(lightDir, norm); // light dir MUST point to fragment
		float spec = pow(max(dot(-viewDir, reflectedLight), 0.0), shininess);

		vec3 specular = specularStr * spec * lampColor;

		fragColor = vec4(specular * objectColor, 1.0);
	}
	else { // Combined - Phong
		vec3 ambient = lampColor * ambientStr;

		vec3 norm = normalize(vecNormal);
		vec3 lightDir = normalize(fragPos - lampPos);
		float diff = -1 * min(0.0, dot(norm, lightDir));
		vec3 diffuse = diff * lampColor;

		vec3 viewDir = normalize(fragPos - viewerPos);
		vec3 reflectedLight = reflect(lightDir, norm); // light dir MUST point to fragment
		float spec = pow(max(dot(-viewDir, reflectedLight), 0.0), shininess);
		vec3 specular = specularStr * spec * lampColor;

		vec3 result = (ambient + diffuse + specular) * objectColor;
		fragColor = vec4(result, 1.0);
	}
}