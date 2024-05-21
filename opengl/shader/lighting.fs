#version 430 core
layout (location = 0) out vec4 fragColor;

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoord;
} fs_in;

#include "include/default.incl"

#define texture_ambient m_ambient
#define texture_diffuse m_diffuse
#define texture_specular m_specular
#define texture_normal m_normal
#define texture_height m_height
#define texture_tangent m_tangent

#define shineness m_shineness

#define light l_lights[0]

#define DIRECTIONAL (0)
#define POINT (1)
#define SPOT (2)

#define viewPos c_view_position

vec3 calcAmbient() {
    vec3  texColor = texture(texture_ambient, fs_in.texCoord).xyz;

    return texColor * light.ambient;
}

vec3 calcDiffuse(vec3 normal, vec3 lightDir) {
    vec3  texColor = texture(texture_diffuse, fs_in.texCoord).xyz;
    float diff = max(dot(normal, lightDir), 0.0);

    return diff * texColor * light.diffuse;
}

vec3 calcSpecular(vec3 normal, vec3 lightDir) {
    vec3    specColor = texture(texture_specular, fs_in.texCoord).xyz;
    float   spec      = 0.0;
    vec3    viewDir   = normalize(viewPos - fs_in.position);
    vec3    halfDir   = normalize(lightDir + viewDir);

    spec = pow(max(dot(halfDir, normal), 0.0), shineness);

    return spec * specColor * light.specular;
}

vec3 directionalLight() {
    vec3  lightDir = normalize(-light.direction);
    vec3  normal   = normalize(fs_in.normal);

    vec3  ambient  = calcAmbient();
    vec3  diffuse  = calcDiffuse(normal, lightDir);
    vec3  specular = calcSpecular(normal, lightDir);

    return ambient + diffuse + specular;
}

float calcAttenuation(float dist) {
    return (1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist)));
}

vec3 pointLight() {
    float   dist        = length(light.position - fs_in.position);
    float   attenuation = calcAttenuation(dist);
    vec3    lightDir    = (light.position - fs_in.position) / dist;
    vec3    normal      = normalize(fs_in.normal);

    vec3    ambient   = calcAmbient();
    vec3    diffuse   = calcDiffuse(normal, lightDir);
    vec3    specular  = calcSpecular(normal, lightDir);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 spotLight() {
    float   dist        = length(light.position - fs_in.position);
    float   attenuation = calcAttenuation(dist);
    vec3    lightDir    = (light.position - fs_in.position) / dist;

    vec3 result = calcAmbient();

    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutoff[0] - light.cutoff[1];
    float intensity = clamp((theta - light.cutoff[1]) / epsilon, 0.0, 1.0);

    if (intensity > 0.0) {
        vec3  normal    = normalize(fs_in.normal);
        vec3  diffuse   = calcDiffuse(normal, lightDir);
        vec3  specular  = calcSpecular(normal, lightDir);
        result += (diffuse + specular) * intensity;
    }
    result *= attenuation;

    return result;
}

void main() {
    vec3 result;

    if (light.type == DIRECTIONAL) {
        result = directionalLight();
    } else if (light.type == POINT) {
        result = pointLight();
    } else {
        result = spotLight();
    }

    fragColor = vec4(result, 1.0);
}
