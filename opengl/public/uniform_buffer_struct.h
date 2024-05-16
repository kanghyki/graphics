#ifndef INCLUDED_UNIFORM_BUFFER_STRUCT_H
#define INCLUDED_UNIFORM_BUFFER_STRUCT_H

#include "libmath.h"

struct MatricesUBO
{
    glm::mat4 t_view;
    glm::mat4 t_proj;
    glm::mat4 t_model;
};

struct MaterialUBO
{
    int m_int_0;
    int m_int_1;
    int m_int_2;
    int m_int_3;

    float m_float_0;
    float m_float_1;
    float m_float_2;
    float m_float_3;

    glm::vec2 m_vec2_0;
    glm::vec2 m_vec2_1;
    glm::vec2 m_vec2_2;
    glm::vec2 m_vec2_3;

    glm::vec4 m_vec4_0;
    glm::vec4 m_vec4_1;
    glm::vec4 m_vec4_2;
    glm::vec4 m_vec4_3;

    glm::mat4 m_mat4_0;
    glm::mat4 m_mat4_1;
    glm::mat4 m_mat4_2;
    glm::mat4 m_mat4_3;
};

struct LightGLSL
{
    // Point & Spot
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    // Directional & Spot
    glm::vec3 direction;

    // Spot
    glm::vec2 cutoff;

    // All
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    int type;
};

struct LightsUBO
{
    int l_light_count;
    LightGLSL l_lights[32];
};

struct GlobalUBO
{
    glm::vec2 g_res;
    float g_dt;
};

#endif