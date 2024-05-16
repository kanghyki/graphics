#ifndef INCLUDED_OPENGL_DEVICE_H
#define INCLUDED_OPENGL_DEVICE_H

#include "buffer.h"
#include "framebuffer.h"
#include "graphics_pso.h"
#include "libopengl.h"
#include "mesh.h"
#include "program.h"
#include "uniform_buffer_struct.h"
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

    /* GLFW */
    GLFWwindow *glfw_window_;

    /* framebuffer */
    FramebufferPtr main_framebuffer_;

    /* PSO */
    GraphicsPSO lighting_pso_;
    GraphicsPSO simple_pso_;

    /* shader */
    ShaderPtr lighting_vs_;
    ShaderPtr lighting_fs_;

    ShaderPtr simple_vs_;
    ShaderPtr simple_fs_;

    /* program */
    ProgramPtr lighting_program_;
    ProgramPtr simple_program_;

    /* uniform buffer */
    BufferPtr matrices_ubo_;
    BufferPtr material_ubo_;
    BufferPtr lights_ubo_;
    BufferPtr global_ubo_;

    /* Post processing */
    MeshPtr plane_mesh_;
    ShaderPtr post_processing_vs_;
    ShaderPtr post_processing_fs_;
    ProgramPtr post_processing_program_;
    GraphicsPSO post_processing_pso_;
};

#endif
