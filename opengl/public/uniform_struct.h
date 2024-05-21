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

struct LightUniform
{
    int type;
    float constant;
    float linear;
    float quadratic;
    glm::vec2 cutoff;
    unsigned char __p2[8];
    glm::vec3 position;
    unsigned char __p0[4];
    glm::vec3 direction;
    unsigned char __p1[4];
    glm::vec3 ambient;
    unsigned char __p3[4];
    glm::vec3 diffuse;
    unsigned char __p4[4];
    glm::vec3 specular;
    unsigned char __p5[4];
};

struct LightsUniform
{
    enum
    {
        LIGHT_MAX = 64
    };
    int count{0};
    unsigned char __p0[12];
    LightUniform l_lights[LIGHT_MAX];
};

struct GlobalUniform
{
    glm::vec2 g_resolution;
    float g_time;
    float g_delta_time;
};

#endif
