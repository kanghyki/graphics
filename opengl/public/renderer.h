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
    MAIN,
    G_BUFFER,
    SKYBOX,
    DEPTH_MAP,
    OMNI_DEPTH_MAP
};

enum class FramebufferType
{
    MAIN,
    G_BUFFER,
    GAUSSIAN_BLUR
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
    void RenderDeffered();
    void PostProcessing();

    void ApplyPSO(const GraphicsPSOPtr &pso) const;

    void Resize(int width, int height);
    int width()
    {
        return width_;
    }
    int height()
    {
        return height_;
    }
    float aspect()
    {
        return aspect_;
    }
    MeshPtr plane_mesh()
    {
        return plane_mesh_;
    }
    MeshPtr box_mesh()
    {
        return box_mesh_;
    }

    GraphicsPSOPtr GetPSO(PsoType type)
    {
        switch (type)
        {
        case PsoType::G_BUFFER:
            return g_buffer_pso_;
        case PsoType::SKYBOX:
            return skybox_pso_;
        case PsoType::DEPTH_MAP:
            return depth_map_pso_;
        case PsoType::OMNI_DEPTH_MAP:
            return omni_depth_map_pso_;
        }
        return nullptr;
    }
    FramebufferPtr GetFramebuffer(FramebufferType type)
    {
        switch (type)
        {
        case FramebufferType::MAIN:
            return main_framebuffer_;
        case FramebufferType::G_BUFFER:
            return g_buffer_;
        case FramebufferType::GAUSSIAN_BLUR:
            return gaussian_blur_framebuffer_[0];
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
    float gamma_{1.0f};
    bool use_gray_scale_{false};
    bool use_bloom_{false};
    float bloom_strength_{1.0f};
    float exposure_{1.0f};
    int blur_time_{5};

  private:
    Renderer();
    Renderer(const Renderer &rhs);
    Renderer &operator=(const Renderer &rhs);

    static Renderer *instance_;

    int width_;
    int height_;
    float aspect_;

    MeshPtr plane_mesh_;
    MeshPtr box_mesh_;

    FramebufferPtr main_framebuffer_;

    ShaderPtr g_buffer_vs_;
    ShaderPtr g_buffer_fs_;
    ProgramPtr g_buffer_program_;
    GraphicsPSOPtr g_buffer_pso_;

    FramebufferPtr g_buffer_;
    ShaderPtr deffered_shading_vs_;
    ShaderPtr deffered_shading_fs_;
    ProgramPtr deffered_shading_program_;
    GraphicsPSOPtr deffered_shading_pso_;

    ShaderPtr skybox_vs_;
    ShaderPtr skybox_fs_;
    ProgramPtr skybox_program_;
    GraphicsPSOPtr skybox_pso_;

    ShaderPtr depth_map_vs_;
    ShaderPtr depth_map_fs_;
    ProgramPtr depth_map_program_;
    GraphicsPSOPtr depth_map_pso_;

    ShaderPtr omni_depth_map_vs_;
    ShaderPtr omni_depth_map_fs_;
    ProgramPtr omni_depth_map_program_;
    GraphicsPSOPtr omni_depth_map_pso_;

    FramebufferPtr gaussian_blur_framebuffer_[2];
    ShaderPtr gaussian_blur_vs_;
    ShaderPtr gaussian_blur_fs_;
    ProgramPtr gaussian_blur_program_;
    GraphicsPSOPtr gaussian_blur_pso_;

    ShaderPtr post_processing_vs_;
    ShaderPtr post_processing_fs_;
    ProgramPtr post_processing_program_;
    GraphicsPSOPtr post_processing_pso_;

    BufferPtr camera_ubo_;
    BufferPtr matrices_ubo_;
    BufferPtr lights_ubo_;
    BufferPtr global_ubo_;
    BufferPtr material_ubo_;
};

#endif