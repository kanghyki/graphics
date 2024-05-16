#include "graphics_pso.h"

GraphicsPSO::GraphicsPSO() : program_(nullptr), framebuffer_(nullptr), rasterizer_state_(), camera_uniform_(nullptr)
{
}

GraphicsPSO::GraphicsPSO(const GraphicsPSO &other)
{
    program_ = other.program_;
    framebuffer_ = other.framebuffer_;
    rasterizer_state_ = other.rasterizer_state_;
    camera_uniform_ = other.camera_uniform_;
}

GraphicsPSO &GraphicsPSO::operator=(const GraphicsPSO &other)
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

GraphicsPSO::~GraphicsPSO()
{
}