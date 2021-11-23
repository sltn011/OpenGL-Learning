#version 330

in vec2 vertexTex;

out float occlusion;



uniform sampler2D gWorldPosition;
uniform sampler2D gNormal;

uniform int imageWidth;
uniform int imageHeight;

uniform sampler1D samples;
uniform int numSamples;

uniform sampler2D noiseRotation;

uniform mat4 view;
uniform mat4 projection;



void main() {
    const float radius = 0.2;

    vec2 noiseScale = vec2(imageWidth / 4, imageHeight / 4);

    vec3 fragPos    = texture(gWorldPosition, vertexTex).xyz;
    vec3 fragNormal = normalize(texture(gNormal, vertexTex).xyz);
    vec3 randomVec  = texture(noiseRotation, vertexTex * noiseScale).xyz;

    vec3 fragPosVS = vec3(view * vec4(fragPos, 1.0));
    vec3 fragNormalVS = normalize(vec3(view * vec4(fragNormal, 0.0)));

    vec3 tangent   = normalize(randomVec - fragNormalVS * dot(randomVec, fragNormalVS));
    vec3 bitangent = cross(fragNormalVS, tangent);
    mat3 TBN       = mat3(tangent, bitangent, fragNormalVS);  

    occlusion = 0.0;

    for (int i = 0; i < numSamples; ++i) {
        vec3 samplePos = TBN * texture(samples, float(i) / float(numSamples)).xyz;
    
        if (dot(samplePos, fragNormalVS) < 0.15) continue;
        samplePos = fragPosVS + samplePos * radius;
        
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;
    
        float sampleDepth = (view * vec4(texture(gWorldPosition, offset.xy).xyz, 1.0)).z;
    
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPosVS.z - sampleDepth));

        occlusion += (sampleDepth >= fragPosVS.z + 0.015 ? 1.0 : 0.0) * rangeCheck;  
    }
    occlusion = 1.0 - (occlusion / float(numSamples));
}