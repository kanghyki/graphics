#include "renderer.h"

Renderer *Renderer::instance_ = nullptr;

Renderer *Renderer::GetInstance()
{
    if (!instance_)
    {
        instance_ = new Renderer();
    }
    return instance_;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
    // TODO:
    std::string shader_dir(SHADER_PATH);
    /* shader */
    lighting_vs_ = Shader::CreateFromFile(shader_dir + "lighting.vs", GL_VERTEX_SHADER);
    lighting_fs_ = Shader::CreateFromFile(shader_dir + "lighting.fs", GL_FRAGMENT_SHADER);

    simple_vs_ = Shader::CreateFromFile(shader_dir + "simple.vs", GL_VERTEX_SHADER);
    simple_fs_ = Shader::CreateFromFile(shader_dir + "simple.fs", GL_FRAGMENT_SHADER);

    /* program */
    lighting_program_ = Program::Create({lighting_vs_, lighting_fs_});
    simple_program_ = Program::Create({simple_vs_, simple_fs_});

    /* framebuffer */
    main_framebuffer_ =
        Framebuffer::Create({Texture2d::Create(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, GL_RGBA16F, GL_RGBA, GL_FLOAT)});

    /* PSO */
    lighting_pso_.framebuffer_ = main_framebuffer_;
    lighting_pso_.program_ = lighting_program_;
    lighting_pso_.rasterizer_state_.draw_type = RasterizerState::DrawType::FILL;
    lighting_pso_.rasterizer_state_.cull_type = RasterizerState::Face::BACK;

    lighting_wire_pso_ = lighting_pso_;
    lighting_wire_pso_.rasterizer_state_.draw_type = RasterizerState::DrawType::WIRE;

    simple_pso_.framebuffer_ = main_framebuffer_;
    simple_pso_.program_ = simple_program_;
    simple_pso_.rasterizer_state_.draw_type = RasterizerState::DrawType::FILL;
    simple_pso_.rasterizer_state_.cull_type = RasterizerState::Face::BACK;
}

void Renderer::Render(ModelPtr model, const GraphicsPSO &pso)
{
    glPolygonMode(GL_FRONT_AND_BACK, static_cast<uint32_t>(pso.rasterizer_state_.draw_type));
    if (pso.rasterizer_state_.cull_type == RasterizerState::Face::NONE)
    {
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glEnable(GL_CULL_FACE);
        glCullFace(static_cast<uint32_t>(pso.rasterizer_state_.cull_type));
    }
    pso.framebuffer_->Bind();
    pso.program_->Use();
    model->Draw(pso.program_.get());
}

GraphicsPSO Renderer::GetPSO(GRAPHIC_PSO_TYPE type)
{
    switch (type)
    {
    case GRAPHIC_PSO_TYPE::LIGHTING:
        return lighting_pso_;
    case GRAPHIC_PSO_TYPE::SIMPLE:
        return simple_pso_;
    }
}
