#ifndef INCLUDED_GRAPHICS_PSO_H
#define INCLUDED_GRAPHICS_PSO_H

#include "buffer.h"
#include "framebuffer.h"
#include "program.h"

struct BlendState
{
    bool is_active_;
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
    DrawType draw_type_{DrawType::FILL};

    Face cull_face_{Face::BACK};
};

struct GraphicsPSO
{
    GraphicsPSO() : program_(nullptr), framebuffer_(nullptr), rasterizer_state_(), camera_uniform_(nullptr)
    {
    }
    GraphicsPSO(const GraphicsPSO &other)
    {
        program_ = other.program_;
        framebuffer_ = other.framebuffer_;
        rasterizer_state_ = other.rasterizer_state_;
        camera_uniform_ = other.camera_uniform_;
    }

    GraphicsPSO &operator=(const GraphicsPSO &other)
    {
        if (this != &other)
        {
            program_ = other.program_;
            framebuffer_ = other.framebuffer_;
            rasterizer_state_ = other.rasterizer_state_;
            camera_uniform_ = other.camera_uniform_;
        }
        return *this;
    }
    ProgramPtr program_;
    FramebufferPtr framebuffer_;
    RasterizerState rasterizer_state_;

    BufferPtr camera_uniform_;
};

#endif
