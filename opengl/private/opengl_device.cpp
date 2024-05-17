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

    if (!glfwExtensionSupported("GL_ARB_shading_language_include"))
    {
        SPDLOG_ERROR("GL_ARB_shading_language_include is not supported");
        return false;
    }

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

    // auto BindUniform = [](ProgramPtr program) {
    //     uint32_t matrices_block_index = glGetUniformBlockIndex(program->id(), "Matrices");
    //     uint32_t material_block_index = glGetUniformBlockIndex(program->id(), "Material");
    //     uint32_t lights_block_index = glGetUniformBlockIndex(program->id(), "Lights");
    //     uint32_t global_block_index = glGetUniformBlockIndex(program->id(), "Global");
    //     SPDLOG_INFO("{} {} {} {}", matrices_block_index, material_block_index, lights_block_index,
    //     global_block_index); glUniformBlockBinding(program->id(), matrices_block_index, 0);
    //     glUniformBlockBinding(program->id(), material_block_index, 1);
    //     glUniformBlockBinding(program->id(), lights_block_index, 2);
    //     glUniformBlockBinding(program->id(), global_block_index, 3);
    // };
    // uniform_bind(lighting_program_->id());
    // BindUniform(simple_program_);

    /* framebuffer */
    main_framebuffer_ = Framebuffer::Create({Texture2d::Create(width_, height_)});

    /* PSO */
    lighting_pso_.program_ = lighting_program_;
    lighting_pso_.framebuffer_ = main_framebuffer_;

    simple_pso_.program_ = simple_program_;
    simple_pso_.framebuffer_ = main_framebuffer_;
    simple_pso_.rasterizer_state_.polygon_mode = GL_LINE;

    post_processing_pso_.rasterizer_state_.is_depth_test_ = false;

    /* Post processing */
    plane_mesh_ = Mesh::CreatePlane();
    // plane_mesh_->set_material(Material::Create());
    // plane_mesh_->material()->set_texture(TextureType::AMBIENT, main_framebuffer_->color_attachment(0));
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

GLFWwindow *OpenGLDevice::glfw_window()
{
    return glfw_window_;
}

void OpenGLDevice::ApplyPSO(const GraphicsPSO &pso)
{
    uint32_t enabled = 0;
    if (pso.rasterizer_state_.is_cull_face_)
    {
        enabled |= GL_CULL_FACE;
        glCullFace(pso.rasterizer_state_.cull_face_);
    }
    if (pso.rasterizer_state_.is_blend_)
    {
        enabled |= GL_BLEND;
    }
    if (pso.rasterizer_state_.is_depth_test_)
    {
        enabled |= GL_DEPTH_TEST;
    }
    if (pso.rasterizer_state_.is_stencil_test_)
    {
        enabled |= GL_STENCIL;
    }
    glDisable(GL_CULL_FACE | GL_BLEND | GL_DEPTH_TEST | GL_STENCIL);
    glEnable(enabled);
    glPolygonMode(GL_FRONT_AND_BACK, pso.rasterizer_state_.polygon_mode);
}