#include "event_callback.h"
#include "editor.h"
#include "input_manager.h"
#include "opengl_device.h"
#include "renderer.h"
#include "resource_manager.h"
#include <spdlog/spdlog.h>

void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void CharCallback(GLFWwindow *window, unsigned int ch);
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void CursorCallback(GLFWwindow *window, double x, double y);
void MouseCallback(GLFWwindow *window, int button, int action, int modifier);
#ifdef EDITOR
void DropCallback(GLFWwindow *window, int count, const char **paths);
#endif

void event::glfw_callback_init()
{
    GLFWwindow *window = OpenGLDevice::GetInstance()->glfw_window();

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCharCallback(window, CharCallback);
    glfwSetCursorPosCallback(window, CursorCallback);
    glfwSetMouseButtonCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
#ifdef EDITOR
    glfwSetDropCallback(window, DropCallback);
#endif
}

void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    Renderer::GetInstance()->Resize(width, height);
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
#ifdef EDITOR
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
#endif
    InputManager::GetInstance()->UpdateKey(key, action);
}

void CharCallback(GLFWwindow *window, unsigned int ch)
{
#ifdef EDITOR
    ImGui_ImplGlfw_CharCallback(window, ch);
#endif
}

void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
#ifdef EDITOR
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
#endif
}

void CursorCallback(GLFWwindow *window, double x, double y)
{
    InputManager::GetInstance()->UpdateCursor(x, y);
}

void MouseCallback(GLFWwindow *window, int button, int action, int modifier)
{
#ifdef EDITOR
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);
#endif
    InputManager::GetInstance()->UpdateMouse(button, action);
}

#ifdef EDITOR
void DropCallback(GLFWwindow *window, int count, const char **paths)
{
    for (int i = 0; i < count; ++i)
    {
        SPDLOG_INFO("drag & drop: {}", paths[i]);
        bool ret = ResourceManager::GetInstance()->LoadResource(paths[i]);
    }
}
#endif