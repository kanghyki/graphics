#ifndef INCLUDED_UNIFORM_STRUCT_H
#define INCLUDED_UNIFORM_STRUCT_H

#include "libmath.h"

/*
 * "include/defualt.incl"
 * Must keep in sync with GLSL
 */

struct MatricesUniform
{
    void Sub() const;
    glm::mat4 t_view;
    glm::mat4 t_proj;
    glm::mat4 t_model;
};

struct MaterialUniform
{
    void Sub() const;
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

struct LightUniform
{
    void Sub() const;
    // Point & Spot
    int type;        // 4
    float constant;  // 4
    float linear;    // 4
    float quadratic; // 4 (16)

    glm::vec3 position; // 12
    float padding1;

    // Directional & Spot
    glm::vec3 direction; // 12
    float padding2;

    // Spot
    glm::vec2 cutoff; // 8
    float padding3[2];

    // All
    glm::vec3 ambient; // 12
    float padding4;
    glm::vec3 diffuse; // 12
    float padding5;
    glm::vec3 specular; // 12
    float padding6;
};

struct LightsUniform
{
    LightUniform l_lights;
};

struct GlobalUniform
{
    void Sub() const;
    glm::vec2 g_resolution;
    float g_time;
    float g_delta_time;
};

#endif
