#version 430 core
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;
in vec2 texCoord;

#include "include/default.incl"
#include "include/blinn.incl"

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gEmissive;

uniform bool use_ssao;
uniform sampler2D SSAO;

void main()
{
    vec3    frag_pos    = texture(gPosition, texCoord).rgb;
    vec3    normal      = texture(gNormal, texCoord).rgb;
    vec3    albedo      = texture(gAlbedoSpec, texCoord).rgb;
    float   specular    = texture(gAlbedoSpec, texCoord).a;
    vec3    to_eye      = normalize(c_view_position - frag_pos);
    vec3    emissive    = texture(gEmissive, texCoord).rgb;

    MaterialForShading mat;
    mat.albedo = albedo;
    mat.specular_alpha = specular;
    mat.shininess = material.shininess;

    vec3 lighting = emissive;
    lighting += use_ssao ? mat.albedo * 0.1 * texture(SSAO, texCoord).r : mat.albedo *  0.1;
    for (int i = 0; i < l_light_count; ++i)
    {
        if (l_lights[i].type == DIRECTIONAL)
        {
            lighting += ComputeDirectionalLight(l_lights[i], mat, frag_pos, normal, to_eye);
        }
        else if (l_lights[i].type == POINT)
        {
            lighting += ComputePointLight(l_lights[i], mat, frag_pos, normal, to_eye);
        }
        else if (l_lights[i].type == SPOT)
        {
            lighting += ComputeSpotLight(l_lights[i], mat, frag_pos, normal, to_eye);
        }
    }

    fragColor = vec4(lighting, 1.0);

    float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
    {
        brightColor = vec4(fragColor.rgb, 1.0);
    }
    else
    {
        brightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}