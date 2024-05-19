#include "opengl_device.h"
#include <spdlog/spdlog.h>

OpenGLDevice *OpenGLDevice::instance_ = nullptr;

OpenGLDevice *OpenGLDevice::GetInstance()
{
    if (!instance_)
    {
        instance_ = new OpenGLDevice();
    }
    return instance_;
}

OpenGLDevice ::OpenGLDevice()
    : width_(WINDOW_INIT_WIDTH), height_(WINDOW_INIT_HEIGHT), window_name_(WINDOW_NAME), glfw_window_(nullptr)
{
}

OpenGLDevice ::~OpenGLDevice()
{
}

bool OpenGLDevice::Init()
{
    if (!glfwInit())
    {
        const char *desc = nullptr;
        glfwGetError(&desc);
        SPDLOG_ERROR("failed to initialize glfw: {}", desc);
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef __OSX__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfw_window_ = glfwCreateWindow(width_, height_, window_name_.c_str(), nullptr, nullptr);
    if (!glfw_window_)
    {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(glfw_window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SPDLOG_ERROR("failed to initialize GLAD");
        glfwTerminate();
        return false;
    }
    glViewport(0, 0, width_, height_);

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
    matrices_ubo_ = Buffer::Create(GL_UNIFORM_BUFFER, GL_STATIC_DRAW, NULL, sizeof(MatricesUniform), 1);
    material_ubo_ = Buffer::Create(GL_UNIFORM_BUFFER, GL_STATIC_DRAW, NULL, sizeof(MaterialUniform), 1);
    lights_ubo_ = Buffer::Create(GL_UNIFORM_BUFFER, GL_STATIC_DRAW, NULL, sizeof(LightsUniform), 1);
    global_ubo_ = Buffer::Create(GL_UNIFORM_BUFFER, GL_STATIC_DRAW, NULL, sizeof(GlobalUniform), 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, matrices_ubo_->id());
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, material_ubo_->id());
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, lights_ubo_->id());
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, global_ubo_->id());

    /* framebuffer */
    main_framebuffer_ = Framebuffer::Create({Texture2d::Create(width_, height_)});

    /* PSO */
    lighting_pso_ = GraphicsPSO::Create();
    simple_pso_ = GraphicsPSO::Create();
    post_processing_pso_ = GraphicsPSO::Create();

    lighting_pso_->program_ = lighting_program_;
    simple_pso_->program_ = simple_program_;
    post_processing_pso_->rasterizer_state_.is_depth_test_ = false;

    /* Post processing */
    plane_mesh_ = Mesh::CreatePlane();
    post_processing_vs_ = Shader::CreateFromFile(shader_dir + "post_processing.vs", GL_VERTEX_SHADER);
    post_processing_fs_ = Shader::CreateFromFile(shader_dir + "post_processing.fs", GL_FRAGMENT_SHADER);
    post_processing_program_ = Program::Create({post_processing_vs_, post_processing_fs_});

    return true;
}

bool OpenGLDevice::IsWindowClose()
{
    return glfwWindowShouldClose(glfw_window_);
}

void OpenGLDevice::SwapBuffer()
{
    glfwSwapBuffers(glfw_window_);
}

void OpenGLDevice::Terminate()
{
    glfwTerminate();
}

void OpenGLDevice::ClearFramebuffer()
{
    main_framebuffer_->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    Framebuffer::BindToDefault();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

GraphicsPSOPtr OpenGLDevice::GetPSO(GRAPHIC_PSO_TYPE type)
{
    switch (type)
    {
    case GRAPHIC_PSO_TYPE::LIGHTING:
        return lighting_pso_;
    case GRAPHIC_PSO_TYPE::SIMPLE:
        return simple_pso_;
    default:
        return simple_pso_;
    }
}

void OpenGLDevice::ApplyPSO(GraphicsPSOPtr pso) const
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
}
