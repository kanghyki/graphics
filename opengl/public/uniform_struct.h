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
    int shadow_id;

    glm::vec3 color;
    float strength;

    glm::vec3 direction;
    int use_shadow;

    glm::mat4 view;
    glm::mat4 proj;

    float far_plane;
    unsigned char __p0[12];
};

struct LightsUniform
{
    enum
    {
        LIGHT_MAX = 64,
        SHADOW_2D_MAX = 8,
        SHADOW_CUBE_MAX = 8
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
    int use_height_map;
    int use_roughness_map;
    int use_metallic_map;

    int use_emissive_map;
    float specular_alpha;
    float height_scale;
    unsigned char __p0[4];

    glm::vec3 albedo_color;
    unsigned char __p1[4];
};

#endif
