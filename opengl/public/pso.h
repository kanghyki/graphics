#ifndef INCLUDED_GRAPHICS_PSO_H
#define INCLUDED_GRAPHICS_PSO_H

#include "framebuffer.h"
#include "program.h"

struct BlendState
{
};

struct DepthStencilState
{
};

struct RasterizerState
{
    enum class Face
    {
        NONE = GL_NONE,
        FACE = GL_FRONT,
        BACK = GL_BACK,
        FACE_AND_BACK = GL_FRONT_AND_BACK
    };
    enum class DrawType
    {
        DOT = GL_POINT,
        WIRE = GL_LINE,
        FILL = GL_FILL
    };
    Face draw_face_{Face::FACE_AND_BACK};
    DrawType draw_type{DrawType::FILL};

    Face cull_type{Face::BACK};
};

struct GraphicsPSO
{
    ProgramPtr program_;
    FramebufferPtr framebuffer_;
    RasterizerState rasterizer_state_;
};

#endif
