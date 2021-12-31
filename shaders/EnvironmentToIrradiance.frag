#version 330 core

in vec3 texCoords;

out vec4 fragColor;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main() {
    
    vec3 irradiance = vec3(0.0f);

    vec3 normal = normalize(texCoords);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, normal));
    up = normalize(cross(normal, right));

    float sampleDelta = 0.025;
    float numSamples = 0.0;

    for (float phi = 0.0; phi < 2 * PI; phi += sampleDelta) {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
            irradiance += textureLod(environmentMap, sampleVec, 0).rgb * cos(theta) * sin(theta);
            ++numSamples;
        }
    }

    irradiance = PI * irradiance * (1.0 / numSamples);
    
    fragColor = vec4(irradiance, 1.0);
}