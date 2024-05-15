#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include "buffer.h"
#include "camera.h"
#include "framebuffer.h"
#include "model.h"
#include "program.h"
#include "pso.h"
#include <array>

enum class GRAPHIC_PSO_TYPE
{
    LIGHTING,
    SIMPLE,
};

class Renderer
{
  public:
    static Renderer *GetInstance();
    ~Renderer();

    void Init();
    void ClearFramebuffer();
    void Draw(ModelPtr model);
    void Render();

    GraphicsPSO GetPSO(GRAPHIC_PSO_TYPE type)
    {
        switch (type)
        {
        case GRAPHIC_PSO_TYPE::LIGHTING:
            return lighting_pso_;
        case GRAPHIC_PSO_TYPE::SIMPLE:
            return simple_pso_;
        }
    }

    void set_cur_camera(Camera *cam)
    {
        cur_camera_ = cam;
    }

    void set_pso(GraphicsPSO pso)
    {
        cur_pso_ = pso;
    }

  private:
    Renderer();
    Renderer(const Renderer &);
    Renderer &operator=(const Renderer &);

    static Renderer *instance_;

    Camera *cur_camera_;
    GraphicsPSO cur_pso_;

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
    BufferPtr camera_uniform_;

    /* Post processing */
    MeshPtr plane_mesh_;
    ShaderPtr post_processing_vs_;
    ShaderPtr post_processing_fs_;
    ProgramPtr post_processing_program_;
    GraphicsPSO post_processing_pso_;
};

#endif
