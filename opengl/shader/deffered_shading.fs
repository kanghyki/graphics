#version 430 core
out vec4 fragColor;
in vec2 texCoord;

#include "include/default.incl"

#define DIRECTIONAL (0)
#define POINT (1)
#define SPOT (2)

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

vec3 calcAmbient(Light light, vec3 color) {
    return color * light.ambient;
}

vec3 calcDiffuse(Light light, vec3 color, vec3 normal, vec3 lightDir) {
    float diff = max(dot(normal, lightDir), 0.0);

    return diff * color * light.diffuse;
}

vec3 calcSpecular(Light light, vec3 color, float spec)
{
    return spec * color * light.specular;
}

float calcAttenuation(Light light, float dist) {
    return (1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist)));
}

void main() {
    vec3 fragPos = texture(gPosition, texCoord).rgb;
    vec3 normal = texture(gNormal, texCoord).rgb;
    vec3 albedo = texture(gAlbedoSpec, texCoord).rgb;
    float specv = texture(gAlbedoSpec, texCoord).a;

    vec3 lighting = albedo * 0.1;
    vec3 viewDir  = normalize(c_view_position - fragPos);
    for(int i = 0; i < l_light_count; ++i) {
        Light light = l_lights[i];
        vec3 ambient  = calcAmbient(light, albedo);
        vec3 specular = calcSpecular(light, albedo, specv);
        if (l_lights[i].type == DIRECTIONAL) {
            vec3  lightDir = normalize(-light.direction);
            vec3  diffuse  = calcDiffuse(light, albedo, normal, lightDir);
            lighting += ambient + diffuse + specular;
        } else {
            float   dist        = length(light.position - fragPos);
            float   attenuation = calcAttenuation(light, dist);
            vec3    lightDir    = (light.position - fragPos) / dist;
            vec3    diffuse  = calcDiffuse(light, albedo, normal, lightDir);
            if (l_lights[i].type == POINT) {
                lighting += (ambient + diffuse + specular) * attenuation;
            }
            else if (l_lights[i].type == SPOT) {
                float   theta       = dot(lightDir, normalize(-light.direction));
                float   epsilon     = light.cutoff[0] - light.cutoff[1];
                float   intensity   = clamp((theta - light.cutoff[1]) / epsilon, 0.0, 1.0);
                vec3    result      = ambient;
                if (intensity > 0.0) {
                    result += (diffuse + specular) * intensity;
                }
                lighting += result * attenuation;
            }
        }
    }

    fragColor = vec4(lighting, 1.0);
}