#ifndef INCLUDED_GRAPHICS_PSO_H
#define INCLUDED_GRAPHICS_PSO_H

#include "buffer.h"
#include "framebuffer.h"
#include "libopengl.h"
#include "program.h"
#include "shader.h"

struct RasterizerState
{
    uint32_t polygon_mode{GL_FILL};

    bool is_cull_face_{true};
    bool is_depth_test_{true};
    bool is_stencil_test_{false};
    bool is_blend_{false};
    uint32_t cull_face_{GL_BACK};
};

class GraphicsPSO
{
  public:
    GraphicsPSO();
    GraphicsPSO(const GraphicsPSO &other);
    GraphicsPSO &operator=(const GraphicsPSO &other);
    ~GraphicsPSO();

    ShaderPtr vs_;
    ShaderPtr fs_;
    ShaderPtr gs_;

    ProgramPtr program_;
    FramebufferPtr framebuffer_;
    RasterizerState rasterizer_state_;

    BufferPtr camera_uniform_;
};

#endif