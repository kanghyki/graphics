#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include "buffer.h"
#include "framebuffer.h"
#include "graphics_pso.h"
#include "mesh.h"
#include "program.h"
#include "shader.h"

enum class PsoType
{
    G_BUFFER,
};

enum class FramebufferType
{
    G_BUFFER,
    MAIN
};

enum class UBOType
{
    CAMERA,
    MATRICES,
    LIGHT,
    GLOBAL,
    MATERIAL
};

class Renderer
{
  public:
    static Renderer *GetInstance();
    ~Renderer();

    void Init();
    void ClearFramebuffer();
    void Render();
    void PostProcessing();

    void ApplyPSO(const GraphicsPSOPtr &pso) const;

    GraphicsPSOPtr GetPSO(PsoType type)
    {
        switch (type)
        {
        case PsoType::G_BUFFER:
            return g_buffer_pso_;
        }
        return nullptr;
    }
    FramebufferPtr GetFramebuffer(FramebufferType type)
    {
        switch (type)
        {
        case FramebufferType::G_BUFFER:
            return g_buffer_;
        case FramebufferType::MAIN:
            return main_framebuffer_;
        }
        return nullptr;
    }
    BufferPtr GetUBO(UBOType type)
    {
        switch (type)
        {
        case UBOType::CAMERA:
            return camera_ubo_;
        case UBOType::MATRICES:
            return matrices_ubo_;
        case UBOType::LIGHT:
            return lights_ubo_;
        case UBOType::GLOBAL:
            return global_ubo_;
        case UBOType::MATERIAL:
            return material_ubo_;
        }
        return nullptr;
    }

    /* Render option */
    float gamma_{1.0f};
    void set_wireframe(bool b)
    {
        if (b)
        {
            g_buffer_pso_->rasterizer_state_.polygon_mode = GL_LINE;
        }
        else
        {
            g_buffer_pso_->rasterizer_state_.polygon_mode = GL_FILL;
        }
    }
    bool gray_scale_{false};

  private:
    Renderer();
    Renderer(const Renderer &rhs);
    Renderer &operator=(const Renderer &rhs);

    static Renderer *instance_;

    /* framebuffer */
    FramebufferPtr main_framebuffer_;
    FramebufferPtr g_buffer_;

    /* shader */
    ShaderPtr g_buffer_vs_;
    ShaderPtr g_buffer_fs_;
    ShaderPtr deffered_shading_vs_;
    ShaderPtr deffered_shading_fs_;
    ShaderPtr post_processing_vs_;
    ShaderPtr post_processing_fs_;

    /* program */
    ProgramPtr g_buffer_program_;
    ProgramPtr deffered_shading_program_;
    ProgramPtr post_processing_program_;

    /* PSO */
    GraphicsPSOPtr g_buffer_pso_;
    GraphicsPSOPtr deffered_shading_pso_;
    GraphicsPSOPtr post_processing_pso_;

    /* uniform buffer */
    BufferPtr camera_ubo_;
    BufferPtr matrices_ubo_;
    BufferPtr lights_ubo_;
    BufferPtr global_ubo_;
    BufferPtr material_ubo_;

    /* For render */
    MeshPtr plane_mesh_;
};

#endif