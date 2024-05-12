#ifndef INCLUDED_RENDER_PARAMS_H
#define INCLUDED_RENDER_PARAMS_H

#include "libmath.h"

enum class LightType
{
    DIRECTIONAL,
    POINT,
    SPOT
};

struct LightParams
{
    LightType type_;
    glm::vec3 position_;
    glm::vec3 direction_;
};

struct TransformParams
{
    glm::mat4 view_;
    glm::mat4 projection_;
    glm::mat4 model_;
};

struct DeviceParams
{
    float current_time_;
    float prev_time_;
    float delta_time_;
    glm::vec2 cursor_position_;
    // current_time_;
    // mouse pos ndc
};

enum class CullingType
{
    FRONT,
    BACK,
    FRONT_AND_BACK
};

enum class DrawType
{
    FILL,
    LINE,
    DOT,
};

struct OpenGLParams
{
    bool use_culling_;
    CullingType culling_type_;
    DrawType draw_type_;
    bool use_depth_test_;
};

struct RenderParams
{
    TransformParams transform_;
    LightParams lights_[16];
    DeviceParams device_;
    OpenGLParams opengl_;
};

#endif
