#include "event_callback.h"
#include "input_manager.h"
#include "opengl_device.h"

void FramebufferCallback(GLFWwindow *window, int width, int height);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void CharCallback(GLFWwindow *window, unsigned int ch);
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void CursorCallback(GLFWwindow *window, double x, double y);
void MouseCallback(GLFWwindow *window, int button, int action, int modifier);

void event::glfw_callback_init()
{
    GLFWwindow *window = OpenGLDevice::GetInstance()->glfw_window();

    glfwSetFramebufferSizeCallback(window, FramebufferCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCharCallback(window, CharCallback);
    glfwSetCursorPosCallback(window, CursorCallback);
    glfwSetMouseButtonCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

void FramebufferCallback(GLFWwindow *window, int width, int height)
{
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    InputManager::GetInstance()->UpdateKey(key, action);
}

void CharCallback(GLFWwindow *window, unsigned int ch)
{
}

void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
}

void CursorCallback(GLFWwindow *window, double x, double y)
{
    InputManager::GetInstance()->UpdateCursor(x, y);
}

void MouseCallback(GLFWwindow *window, int button, int action, int modifier)
{
}
