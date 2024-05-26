#ifndef INCLUDED_UNIFORM_STRUCT_H
#define INCLUDED_UNIFORM_STRUCT_H

#include "libmath.h"

/*
 * "include/defualt.incl"
 * Must keep in sync with GLSL
 */

struct CameraUniform
{
    glm::vec3 c_view_position;
    unsigned char __p0[4];
};

struct MatricesUniform
{
    glm::mat4 t_view;
    glm::mat4 t_proj;
    glm::mat4 t_model;
};

struct LightData
{
    int type;
    float falloff_start;
    float falloff_end;
    float spot_power;

    glm::vec3 position;
    unsigned char __p0[4];

    glm::vec3 color;
    float strength;

    glm::vec3 direction;
    unsigned char __p2[4];
};

struct LightsUniform
{
    enum
    {
        LIGHT_MAX = 64
    };
    int count{0};
    unsigned char __p0[12];
    LightData l_lights[LIGHT_MAX];
};

struct GlobalUniform
{
    glm::vec2 g_resolution;
    float g_time;
    float g_delta_time;
};

struct MaterialUniform
{
    float m_shineness;
    int use_albedo_map;
    int use_specular_map;
    int use_ao_map;

    int use_normal_map;
    int use_roughness_map;
    int use_metallic_map;
    int use_emissive_map;

    glm::vec3 albedo_color;
    float specular_alpha;
};

#endif
