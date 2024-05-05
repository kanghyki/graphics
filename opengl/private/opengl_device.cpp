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

OpenGLDevice ::OpenGLDevice() : width_(1920), height_(1080), window_name_("window"), glfw_window_(nullptr)
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

GLFWwindow *OpenGLDevice::glfw_window()
{
    return glfw_window_;
}
