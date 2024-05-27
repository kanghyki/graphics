#version 430 core
out float fragColor;

in vec2 texCoord;

#include "include/default.incl"

#define view t_view
#define proj t_proj

uniform sampler2D gPosition;
uniform sampler2D gNormal;

uniform sampler2D texNoise;
uniform vec2 noiseScale;
uniform float radius;
uniform float power;

const float BIAS = 0.025;
const int KERNEL_MAX = 64;
uniform vec3 samples[KERNEL_MAX];
uniform int sampleSize;

void main()
{
    vec4 worldPos = texture(gPosition, texCoord);
    if (worldPos.w <= 0.0f)
    {
        discard;
    }
    vec3 fragPos = (view * vec4(worldPos.xyz, 1.0)).xyz;
    vec3 randomVec = texture(texNoise, texCoord * noiseScale).xyz;
    
    vec3 N = (view * vec4(texture(gNormal, texCoord).xyz, 0.0)).xyz;
    vec3 T = normalize(randomVec - N * dot(randomVec, N));
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T, B, N);

    float occlusion = 0.0;
    for (int i = 0; i < sampleSize; ++i)
    {
        vec3 sampleI = fragPos + TBN * samples[i] * radius;
        vec4 screenSample = proj * vec4(sampleI, 1.0);
        screenSample.xyz /= screenSample.w;
        screenSample.xyz = screenSample.xyz * 0.5 + 0.5;
    
        float sampleDepth = (view * texture(gPosition, screenSample.xy)).z;
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sampleI.z + BIAS ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - occlusion / sampleSize;
    fragColor = pow(occlusion, power);
}