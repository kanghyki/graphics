#ifndef INCLUDED_OPENGL_DEVICE_H
#define INCLUDED_OPENGL_DEVICE_H

#include "libopengl.h"
#include <string>

class OpenGLDevice
{
  public:
    static OpenGLDevice *GetInstance();
    ~OpenGLDevice();

    bool Init();
    bool IsWindowClose();
    void SwapBuffer();
    void Terminate();
    GLFWwindow *glfw_window();

  private:
    OpenGLDevice();
    OpenGLDevice(const OpenGLDevice &c);
    OpenGLDevice &operator=(const OpenGLDevice &c);

    static OpenGLDevice *instance_;
    int width_;
    int height_;
    std::string window_name_;

    GLFWwindow *glfw_window_;
};

#endif
