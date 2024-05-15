#include "opengl_device.h"
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    std::string shader_dir(SHADER_PATH);
    /* shader */
    lighting_vs_ = Shader::CreateFromFile(shader_dir + "lighting.vs", GL_VERTEX_SHADER);
    lighting_fs_ = Shader::CreateFromFile(shader_dir + "lighting.fs", GL_FRAGMENT_SHADER);

    simple_vs_ = Shader::CreateFromFile(shader_dir + "simple.vs", GL_VERTEX_SHADER);
    simple_fs_ = Shader::CreateFromFile(shader_dir + "simple.fs", GL_FRAGMENT_SHADER);

    /* program */
    lighting_program_ = Program::Create({lighting_vs_, lighting_fs_});
    simple_program_ = Program::Create({simple_vs_, simple_fs_});

    /* uniform buffer object */
    camera_uniform_ = Buffer::Create(GL_UNIFORM_BUFFER, GL_STATIC_DRAW, NULL, sizeof(glm::mat4), 2);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_uniform_->id());
    glUniformBlockBinding(simple_program_->id(), glGetUniformBlockIndex(simple_program_->id(), "Camera"), 0);
    glUniformBlockBinding(lighting_program_->id(), glGetUniformBlockIndex(lighting_program_->id(), "Camera"), 0);

    /* framebuffer */
    main_framebuffer_ = Framebuffer::Create({Texture2d::Create(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT)});

    /* PSO */
    lighting_pso_.program_ = lighting_program_;
    lighting_pso_.framebuffer_ = main_framebuffer_;
    lighting_pso_.rasterizer_state_.draw_type_ = RasterizerState::DrawType::FILL;
    lighting_pso_.rasterizer_state_.cull_face_ = RasterizerState::Face::BACK;
    lighting_pso_.camera_uniform_ = camera_uniform_;

    simple_pso_.program_ = simple_program_;
    simple_pso_.framebuffer_ = main_framebuffer_;
    simple_pso_.rasterizer_state_.draw_type_ = RasterizerState::DrawType::FILL;
    simple_pso_.rasterizer_state_.cull_face_ = RasterizerState::Face::BACK;
    simple_pso_.camera_uniform_ = camera_uniform_;

    /* Post processing */
    plane_mesh_ = Mesh::CreatePlane();
    post_processing_vs_ = Shader::CreateFromFile(shader_dir + "post_processing.vs", GL_VERTEX_SHADER);
    post_processing_fs_ = Shader::CreateFromFile(shader_dir + "post_processing.fs", GL_FRAGMENT_SHADER);
    post_processing_program_ = Program::Create({post_processing_vs_, post_processing_fs_});
}

void Renderer::ClearFramebuffer()
{
    main_framebuffer_->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    Framebuffer::BindToDefault();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::Draw(ModelPtr model)
{
    GraphicsPSO pso = cur_pso_;

    pso.framebuffer_->Bind();
    glEnable(GL_DEPTH_TEST);

    // TODO:
    glm::mat4 view = cur_camera_->CalcViewMatrix();
    glm::mat4 proj = cur_camera_->CalcPerspectiveProjectionMatrix();

    pso.camera_uniform_->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glPolygonMode(GL_FRONT_AND_BACK, static_cast<uint32_t>(pso.rasterizer_state_.draw_type_));
    if (pso.rasterizer_state_.cull_face_ == RasterizerState::Face::NONE)
    {
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glEnable(GL_CULL_FACE);
        glCullFace(static_cast<uint32_t>(pso.rasterizer_state_.cull_face_));
    }

    simple_program_->Use();
    simple_program_->SetUniform("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    simple_program_->SetUniform("model", glm::mat4(1.0f));
    static MeshPtr test_mesh = Mesh::CreateBox();
    test_mesh->Draw(simple_program_.get());
    // model->Draw(pso.program_.get());
}

void Renderer::Render()
{
    Framebuffer::BindToDefault();
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    post_processing_program_->Use();
    glActiveTexture(GL_TEXTURE0);
    main_framebuffer_->color_attachment(0)->Bind();
    post_processing_program_->SetUniform("main_tex", 0);
    post_processing_program_->SetUniform("model", glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
    plane_mesh_->Draw(post_processing_program_.get());

    OpenGLDevice::GetInstance()->SwapBuffer();
}
