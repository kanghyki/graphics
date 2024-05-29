#include "renderer.h"
#include "opengl_device.h"
#include "random_number_generator.h"
#include <spdlog/spdlog.h>

Renderer *Renderer::instance_ = nullptr;

Renderer *Renderer::GetInstance()
{
    if (!instance_)
    {
        instance_ = new Renderer();
    }
    return instance_;
}

Renderer::Renderer() : width_(WINDOW_INIT_WIDTH), height_(WINDOW_INIT_HEIGHT)
{
    aspect_ = static_cast<float>(width_) / static_cast<float>(height_);
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
    std::string shader_dir(SHADER_PATH);
    glViewport(0, 0, width_, height_);

    plane_mesh_ = Mesh::CreatePlane();
    box_mesh_ = Mesh::CreateBox();

    main_framebuffer_ = Framebuffer::Create(
        {Texture::Create(width_, height_, GL_RGBA16F), Texture::Create(width_, height_, GL_RGBA16F)});

    /* G-Buffer */
    g_buffer_ =
        Framebuffer::Create({Texture::Create(width_, height_, GL_RGBA16F), Texture::Create(width_, height_, GL_RGBA16F),
                             Texture::Create(width_, height_, GL_RGBA), Texture::Create(width_, height_, GL_RGBA16F)});
    g_buffer_vs_ = Shader::CreateFromFile(shader_dir + "g_buffer.vs", GL_VERTEX_SHADER);
    g_buffer_fs_ = Shader::CreateFromFile(shader_dir + "g_buffer.fs", GL_FRAGMENT_SHADER);
    g_buffer_program_ = Program::Create({g_buffer_vs_, g_buffer_fs_});
    g_buffer_pso_ = GraphicsPSO::Create();
    g_buffer_pso_->program_ = g_buffer_program_;

    /* Deffered shading */
    deffered_shading_vs_ = Shader::CreateFromFile(shader_dir + "deffered_shading.vs", GL_VERTEX_SHADER);
    deffered_shading_fs_ = Shader::CreateFromFile(shader_dir + "deffered_shading.fs", GL_FRAGMENT_SHADER);
    deffered_shading_program_ = Program::Create({deffered_shading_vs_, deffered_shading_fs_});
    deffered_shading_pso_ = GraphicsPSO::Create();
    deffered_shading_pso_->program_ = deffered_shading_program_;

    /* Skybox */
    skybox_vs_ = Shader::CreateFromFile(shader_dir + "skybox.vs", GL_VERTEX_SHADER);
    skybox_fs_ = Shader::CreateFromFile(shader_dir + "skybox.fs", GL_FRAGMENT_SHADER);
    skybox_program_ = Program::Create({skybox_vs_, skybox_fs_});
    skybox_pso_ = GraphicsPSO::Create();
    skybox_pso_->program_ = skybox_program_;
    skybox_pso_->rasterizer_state_.cull_face_ = GL_FRONT;
    skybox_pso_->rasterizer_state_.depth_func_ = GL_LEQUAL;

    /* Depth map */
    depth_map_vs_ = Shader::CreateFromFile(shader_dir + "depth_map.vs", GL_VERTEX_SHADER);
    depth_map_fs_ = Shader::CreateFromFile(shader_dir + "depth_map.fs", GL_FRAGMENT_SHADER);
    depth_map_program_ = Program::Create({depth_map_vs_, depth_map_fs_});
    depth_map_pso_ = GraphicsPSO::Create();
    depth_map_pso_->program_ = depth_map_program_;
    depth_map_pso_->rasterizer_state_.cull_face_ = GL_FRONT;

    /* Omni-Depth map */
    omni_depth_map_vs_ = Shader::CreateFromFile(shader_dir + "omni_depth_map.vs", GL_VERTEX_SHADER);
    omni_depth_map_fs_ = Shader::CreateFromFile(shader_dir + "omni_depth_map.fs", GL_FRAGMENT_SHADER);
    omni_depth_map_program_ = Program::Create({omni_depth_map_vs_, omni_depth_map_fs_});
    omni_depth_map_pso_ = GraphicsPSO::Create();
    omni_depth_map_pso_->program_ = omni_depth_map_program_;
    omni_depth_map_pso_->rasterizer_state_.cull_face_ = GL_FRONT;

    /* Gaussian blur */
    gaussian_blur_framebuffer_[0] = Framebuffer::Create({Texture::Create(width_, height_, GL_RGBA16F)});
    gaussian_blur_framebuffer_[1] = Framebuffer::Create({Texture::Create(width_, height_, GL_RGBA16F)});
    gaussian_blur_vs_ = Shader::CreateFromFile(shader_dir + "gaussian_blur.vs", GL_VERTEX_SHADER);
    gaussian_blur_fs_ = Shader::CreateFromFile(shader_dir + "gaussian_blur.fs", GL_FRAGMENT_SHADER);
    gaussian_blur_program_ = Program::Create({gaussian_blur_vs_, gaussian_blur_fs_});
    gaussian_blur_pso_ = GraphicsPSO::Create();
    gaussian_blur_pso_->program_ = gaussian_blur_program_;
    gaussian_blur_pso_->rasterizer_state_.is_depth_test_ = false;

    /* Post processing */
    post_processing_vs_ = Shader::CreateFromFile(shader_dir + "post_processing.vs", GL_VERTEX_SHADER);
    post_processing_fs_ = Shader::CreateFromFile(shader_dir + "post_processing.fs", GL_FRAGMENT_SHADER);
    post_processing_program_ = Program::Create({post_processing_vs_, post_processing_fs_});
    post_processing_pso_ = GraphicsPSO::Create();
    post_processing_pso_->program_ = post_processing_program_;
    post_processing_pso_->rasterizer_state_.is_depth_test_ = false;

    /* SSAO */
    ssao_framebuffer_ = Framebuffer::Create({Texture::Create(width_, height_, GL_RED)});
    ssao_vs_ = Shader::CreateFromFile(shader_dir + "ssao.vs", GL_VERTEX_SHADER);
    ssao_fs_ = Shader::CreateFromFile(shader_dir + "ssao.fs", GL_FRAGMENT_SHADER);
    ssao_program_ = Program::Create({ssao_vs_, ssao_fs_});
    ssao_pso_ = GraphicsPSO::Create();
    ssao_pso_->program_ = ssao_program_;

    ssao_blur_framebuffer_ = Framebuffer::Create({Texture::Create(width_, height_, GL_RED)});
    linear_blur_vs_ = Shader::CreateFromFile(shader_dir + "linear_blur.vs", GL_VERTEX_SHADER);
    linear_blur_fs_ = Shader::CreateFromFile(shader_dir + "linear_blur.fs", GL_FRAGMENT_SHADER);
    linear_blur_program_ = Program::Create({linear_blur_vs_, linear_blur_fs_});
    linear_blur_pso_ = GraphicsPSO::Create();
    linear_blur_pso_->program_ = linear_blur_program_;

    std::vector<glm::vec3> ssao_noise(16);
    for (size_t i = 0; i < ssao_noise.size(); ++i)
    {
        float rx = RandomNumberGenerator().Uniform(-1.0f, 1.0f);
        float ry = RandomNumberGenerator().Uniform(-1.0f, 1.0f);
        ssao_noise[i] = glm::vec3(rx, ry, 0.0f);
    }
    ssao_noise_texture_ = Texture::Create(4, 4, GL_RGB16F);
    ssao_noise_texture_->Bind();
    ssao_noise_texture_->SetFilter(GL_NEAREST, GL_NEAREST);
    ssao_noise_texture_->SetWrap(GL_REPEAT, GL_REPEAT);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 4, 4, GL_RGB, GL_FLOAT, ssao_noise.data());

    ssao_sample_.resize(SSAO_SAMPLE_MAX);
    for (size_t i = 0; i < ssao_sample_.size(); ++i)
    {
        float rx = RandomNumberGenerator().Uniform(-1.0f, 1.0f);
        float ry = RandomNumberGenerator().Uniform(-1.0f, 1.0f);
        float rz = RandomNumberGenerator().Uniform(0.0f, 1.0f);
        float rr = RandomNumberGenerator().Uniform(0.0f, 1.0f);
        glm::vec3 sample = glm::normalize(glm::vec3(rx, ry, rz)) * rr;
        float t = static_cast<float>(i) / static_cast<float>(ssao_sample_.size());
        float t2 = t * t;
        float scale = (1.0f - t2) * 0.1f + t2 * 1.0f;
        ssao_sample_[i] = sample * scale;
    }

    /* Uniform buffer object */
    camera_ubo_ = Buffer::Create(GL_UNIFORM_BUFFER, GL_STATIC_DRAW, NULL, sizeof(CameraUniform), 1);
    matrices_ubo_ = Buffer::Create(GL_UNIFORM_BUFFER, GL_STATIC_DRAW, NULL, sizeof(MatricesUniform), 1);
    lights_ubo_ = Buffer::Create(GL_UNIFORM_BUFFER, GL_STATIC_DRAW, NULL, sizeof(LightsUniform), 1);
    global_ubo_ = Buffer::Create(GL_UNIFORM_BUFFER, GL_STATIC_DRAW, NULL, sizeof(GlobalUniform), 1);
    material_ubo_ = Buffer::Create(GL_UNIFORM_BUFFER, GL_STATIC_DRAW, NULL, sizeof(MaterialUniform), 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_ubo_->id());
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, matrices_ubo_->id());
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, lights_ubo_->id());
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, global_ubo_->id());
    glBindBufferBase(GL_UNIFORM_BUFFER, 4, material_ubo_->id());
}

void Renderer::ClearFramebuffer()
{
    main_framebuffer_->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    g_buffer_->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    if (use_ssao_)
    {
        ssao_framebuffer_->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        ssao_blur_framebuffer_->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    Framebuffer::BindToDefault();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::RenderSSAO()
{
    if (!use_ssao_)
    {
        return;
    }
    ssao_framebuffer_->Bind();
    ApplyPSO(ssao_pso_.get());
    ssao_program_->SetUniform("model", glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
    ssao_program_->ActivateTexture("gPosition", g_buffer_->color_attachment(0));
    ssao_program_->ActivateTexture("gNormal", g_buffer_->color_attachment(1));
    ssao_program_->ActivateTexture("texNoise", ssao_noise_texture_.get());
    ssao_program_->SetUniform("noiseScale", glm::vec2((float)width_ / (float)ssao_noise_texture_->width(),
                                                      (float)height_ / (float)ssao_noise_texture_->height()));
    ssao_program_->SetUniform("radius", ssao_radius_);
    ssao_program_->SetUniform("power", ssao_power_);
    ssao_program_->SetUniform("sampleSize", ssao_sample_size_);
    for (size_t i = 0; i < ssao_sample_size_; ++i)
    {
        auto name = fmt::format("samples[{}]", i);
        ssao_program_->SetUniform(name, ssao_sample_[i]);
    }
    plane_mesh_->Draw(nullptr);

    ssao_blur_framebuffer_->Bind();
    ApplyPSO(linear_blur_pso_.get());
    linear_blur_pso_->program_->ActivateTexture("tex", ssao_framebuffer_->color_attachment(0));
    linear_blur_program_->SetUniform("model", glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
    plane_mesh_->Draw(nullptr);
}

void Renderer::PostProcessing()
{
    if (use_bloom_)
    {
        ApplyPSO(gaussian_blur_pso_.get());
        bool horizontal = true;
        auto model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
        gaussian_blur_program_->SetUniform("transform", model);

        for (unsigned int i = 0; i < blur_time_ * 2; i++)
        {
            gaussian_blur_framebuffer_[horizontal]->Bind();
            gaussian_blur_program_->SetUniform("horizontal", horizontal);
            const BaseTexture *tex = i == 0 ? main_framebuffer_->color_attachment(1)
                                            : gaussian_blur_framebuffer_[!horizontal]->color_attachment(0);
            gaussian_blur_program_->ActivateTexture("image", tex);
            plane_mesh_->Draw(gaussian_blur_program_.get());
            horizontal = !horizontal;
        }
    }

    Framebuffer::BindToDefault();
    ApplyPSO(post_processing_pso_.get());
    post_processing_program_->SetUniform("model", glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
    post_processing_program_->SetUniform("gamma", gamma_);
    post_processing_program_->SetUniform("use_gray_scale", use_gray_scale_);
    post_processing_program_->SetUniform("use_bloom", use_bloom_);
    post_processing_program_->SetUniform("bloom_strength", bloom_strength_);
    post_processing_program_->SetUniform("use_exposure", use_exposure_);
    post_processing_program_->SetUniform("exposure", exposure_);
    post_processing_program_->ActivateTexture("main_texture", main_framebuffer_->color_attachment(0));
    post_processing_program_->ActivateTexture("blured_texture", gaussian_blur_framebuffer_[0]->color_attachment(0));
    plane_mesh_->Draw(nullptr);
}

void Renderer::ApplyPSO(const GraphicsPSO *pso) const
{
    /*
     * cull face
     */
    if (pso->rasterizer_state_.is_cull_face_)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(pso->rasterizer_state_.cull_face_);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    /*
     * blend
     */
    if (pso->rasterizer_state_.is_blend_)
    {
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    /*
     * depth test
     */
    if (pso->rasterizer_state_.is_depth_test_)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(pso->rasterizer_state_.depth_func_);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }

    /*
     * stencil test
     */
    if (pso->rasterizer_state_.is_stencil_test_)
    {
        glEnable(GL_STENCIL_TEST);
    }
    else
    {
        glDisable(GL_STENCIL_TEST);
    }

    /*
     * Polygon
     */
    glPolygonMode(GL_FRONT_AND_BACK, pso->rasterizer_state_.polygon_mode);
    pso->program_->Use();
}

void Renderer::Resize(int width, int height)
{
    if (width == 0 || height == 0)
    {
        return;
    }
    SPDLOG_INFO("Resize window {} {}", width, height);
    width_ = width;
    height_ = height;
    aspect_ = static_cast<float>(width_) / static_cast<float>(height_);
    glViewport(0, 0, width_, height_);
    main_framebuffer_ = Framebuffer::Create(
        {Texture::Create(width_, height_, GL_RGBA16F), Texture::Create(width_, height_, GL_RGBA16F)});
    g_buffer_ =
        Framebuffer::Create({Texture::Create(width_, height_, GL_RGBA16F), Texture::Create(width_, height_, GL_RGBA16F),
                             Texture::Create(width_, height_, GL_RGBA), Texture::Create(width_, height_, GL_RGBA16F)});
    gaussian_blur_framebuffer_[0] = Framebuffer::Create({Texture::Create(width_, height_, GL_RGBA16F)});
    gaussian_blur_framebuffer_[1] = Framebuffer::Create({Texture::Create(width_, height_, GL_RGBA16F)});
    ssao_framebuffer_ = Framebuffer::Create({Texture::Create(width_, height_, GL_RED)});
    ssao_blur_framebuffer_ = Framebuffer::Create({Texture::Create(width_, height_, GL_RED)});
}
