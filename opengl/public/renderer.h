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
    DEFFERED_SHADING,
    SKYBOX,
    DEPTH_MAP,
    OMNI_DEPTH_MAP,
    SSAO
};

enum class FramebufferType
{
    MAIN,
    G_BUFFER,
    GAUSSIAN_BLUR,
    SSAO
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
    void RenderSSAO();
    void PostProcessing();

    void ApplyPSO(const GraphicsPSO *pso) const;
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

    GraphicsPSO *GetPSO(PsoType type)
    {
        switch (type)
        {
        case PsoType::G_BUFFER:
            return g_buffer_pso_.get();
        case PsoType::DEFFERED_SHADING:
            return deffered_shading_pso_.get();
        case PsoType::SKYBOX:
            return skybox_pso_.get();
        case PsoType::DEPTH_MAP:
            return depth_map_pso_.get();
        case PsoType::OMNI_DEPTH_MAP:
            return omni_depth_map_pso_.get();
        case PsoType::SSAO:
            return ssao_pso_.get();
        }
        return nullptr;
    }
    Framebuffer *GetFramebuffer(FramebufferType type)
    {
        switch (type)
        {
        case FramebufferType::MAIN:
            return main_framebuffer_.get();
        case FramebufferType::G_BUFFER:
            return g_buffer_.get();
        case FramebufferType::GAUSSIAN_BLUR:
            return gaussian_blur_framebuffer_[0].get();
        case FramebufferType::SSAO:
            return ssao_framebuffer_.get();
        }
        return nullptr;
    }
    Buffer *GetUBO(UBOType type)
    {
        switch (type)
        {
        case UBOType::CAMERA:
            return camera_ubo_.get();
        case UBOType::MATRICES:
            return matrices_ubo_.get();
        case UBOType::LIGHT:
            return lights_ubo_.get();
        case UBOType::GLOBAL:
            return global_ubo_.get();
        case UBOType::MATERIAL:
            return material_ubo_.get();
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
    bool use_exposure_{false};
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

    /* G-Buffer */
    FramebufferPtr g_buffer_;
    ShaderPtr g_buffer_vs_;
    ShaderPtr g_buffer_fs_;
    ProgramPtr g_buffer_program_;
    GraphicsPSOPtr g_buffer_pso_;

    /* Deffered shading */
    ShaderPtr deffered_shading_vs_;
    ShaderPtr deffered_shading_fs_;
    ProgramPtr deffered_shading_program_;
    GraphicsPSOPtr deffered_shading_pso_;

    /* Skybox */
    ShaderPtr skybox_vs_;
    ShaderPtr skybox_fs_;
    ProgramPtr skybox_program_;
    GraphicsPSOPtr skybox_pso_;

    /* Depth map */
    ShaderPtr depth_map_vs_;
    ShaderPtr depth_map_fs_;
    ProgramPtr depth_map_program_;
    GraphicsPSOPtr depth_map_pso_;

    /* Omni-Depth map */
    ShaderPtr omni_depth_map_vs_;
    ShaderPtr omni_depth_map_fs_;
    ProgramPtr omni_depth_map_program_;
    GraphicsPSOPtr omni_depth_map_pso_;

    /* Gaussian blur */
    FramebufferPtr gaussian_blur_framebuffer_[2];
    ShaderPtr gaussian_blur_vs_;
    ShaderPtr gaussian_blur_fs_;
    ProgramPtr gaussian_blur_program_;
    GraphicsPSOPtr gaussian_blur_pso_;

    /* Post processing */
    ShaderPtr post_processing_vs_;
    ShaderPtr post_processing_fs_;
    ProgramPtr post_processing_program_;
    GraphicsPSOPtr post_processing_pso_;

    /* SSAO */
    FramebufferPtr ssao_framebuffer_;
    ShaderPtr ssao_vs_;
    ShaderPtr ssao_fs_;
    ProgramPtr ssao_program_;
    GraphicsPSOPtr ssao_pso_;
    TexturePtr ssao_noise_texture_;
    std::vector<glm::vec3> ssao_sample_;

  public:
    bool use_ssao_{true};
    const int SSAO_SAMPLE_MAX = 64;
    int ssao_sample_size_{64};
    float ssao_radius_{1.0f};
    float ssao_power_{1.0f};

    FramebufferPtr ssao_blur_framebuffer_;
    ShaderPtr linear_blur_vs_;
    ShaderPtr linear_blur_fs_;
    ProgramPtr linear_blur_program_;
    GraphicsPSOPtr linear_blur_pso_;

  private:
    /* Uniform buffer object */
    BufferPtr camera_ubo_;
    BufferPtr matrices_ubo_;
    BufferPtr lights_ubo_;
    BufferPtr global_ubo_;
    BufferPtr material_ubo_;
};

#endif