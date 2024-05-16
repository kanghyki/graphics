#ifndef INCLUDED_GRAPHICS_PSO_H
#define INCLUDED_GRAPHICS_PSO_H

#include "buffer.h"
#include "framebuffer.h"
#include "program.h"

#include "libopengl.h"

struct RasterizerState
{
    uint32_t polygon_mode{GL_FILL};

    bool is_cull_face_{true};
    bool is_depth_test_{true};
    bool is_stencil_test_{false};
    bool is_blend_{false};
    uint32_t cull_face_{GL_BACK};
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
