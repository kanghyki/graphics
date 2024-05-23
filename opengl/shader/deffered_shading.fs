#version 430 core
out vec4 fragColor;
in vec2 texCoord;

#include "include/default.incl"
#include "include/blinn.incl"

#define DIRECTIONAL (0)
#define POINT (1)
#define SPOT (2)

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main()
{
    vec3    frag_pos    = texture(gPosition, texCoord).rgb;
    vec3    normal      = texture(gNormal, texCoord).rgb;
    vec3    albedo      = texture(gAlbedoSpec, texCoord).rgb;
    float   specular    = texture(gAlbedoSpec, texCoord).a;
    vec3    to_eye      = normalize(c_view_position - frag_pos);

    vec3 lighting;
    MaterialForShading mat;
    mat.albedo = albedo;
    mat.specular_alpha = specular;
    mat.shininess = material.shininess;
    for (int i = 0; i < l_light_count; ++i)
    {
        if (l_lights[i].type == DIRECTIONAL)
        {
            lighting += ComputeDirectionalLight(l_lights[i], mat, normal, to_eye);
        }
        else if (l_lights[i].type == POINT)
        {
            lighting += ComputePointLight(l_lights[i], mat, frag_pos, normal, to_eye);
        }
        else
        {
            lighting += ComputeSpotLight(l_lights[i], mat, frag_pos, normal, to_eye);
        }
    }

    fragColor = vec4(lighting, 1.0);
}