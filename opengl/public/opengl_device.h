#ifndef INCLUDED_OPENGL_DEVICE_H
#define INCLUDED_OPENGL_DEVICE_H

#include "buffer.h"
#include "framebuffer.h"
#include "graphics_pso.h"
#include "libopengl.h"
#include "mesh.h"
#include "program.h"
#include "uniform_struct.h"
#include <string>

enum class GRAPHIC_PSO_TYPE
{
    LIGHTING,
    SIMPLE
};

enum class PROGRAM_TYPE
{
    LIGHTING,
    SIMPLE
};

class OpenGLDevice
{
  public:
    static OpenGLDevice *GetInstance();
    ~OpenGLDevice();

    bool Init();
    bool IsWindowClose();
    void SwapBuffer();
    void Terminate();
    void ClearFramebuffer();
    GLFWwindow *glfw_window();

    GraphicsPSO GetPSO(GRAPHIC_PSO_TYPE type)
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
    ProgramPtr GetProgram(PROGRAM_TYPE type)
    {
        switch (type)
        {
        case PROGRAM_TYPE::LIGHTING:
            return lighting_program_;
        case PROGRAM_TYPE::SIMPLE:
            return simple_program_;
        default:
            return simple_program_;
        }
    }
    void ApplyPSO(const GraphicsPSO &pso);
    BufferPtr matrices_ubo()
    {
        return matrices_ubo_;
    }
    BufferPtr material_ubo()
    {
        return material_ubo_;
    }
    BufferPtr lights_ubo()
    {
        return lights_ubo_;
    }
    BufferPtr global_ubo()
    {
        return global_ubo_;
    }

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
