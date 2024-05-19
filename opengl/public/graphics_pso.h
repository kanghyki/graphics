#ifndef INCLUDED_GRAPHICS_PSO_H
#define INCLUDED_GRAPHICS_PSO_H

#include "buffer.h"
#include "framebuffer.h"
#include "libopengl.h"
#include "program.h"
#include "ptr.h"
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

CLASS_PTR(GraphicsPSO);
class GraphicsPSO
{
  public:
    static GraphicsPSOPtr Create()
    {
        return GraphicsPSOPtr(new GraphicsPSO());
    }

    GraphicsPSO(const GraphicsPSO &rhs)
    {
        program_ = rhs.program_;
        rasterizer_state_ = rhs.rasterizer_state_;
    }

    GraphicsPSO &operator=(const GraphicsPSO &rhs)
    {
        if (this != &rhs)
        {
            program_ = rhs.program_;
            rasterizer_state_ = rhs.rasterizer_state_;
        }
        return *this;
    }

    ~GraphicsPSO()
    {
    }

    ShaderPtr vs_{nullptr};
    ShaderPtr fs_{nullptr};
    ShaderPtr gs_{nullptr};

    ProgramPtr program_{nullptr};
    RasterizerState rasterizer_state_;

  private:
    GraphicsPSO()
    {
    }
};

#endif