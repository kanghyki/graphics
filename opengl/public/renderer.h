#ifndef INCLUDED_RENDERER_H
#define INCLUDED_RENDERER_H

#include "framebuffer.h"
#include "model.h"
#include "program.h"
#include "pso.h"

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
    void Render(ModelPtr model, const GraphicsPSO &pso);
    GraphicsPSO GetPSO(GRAPHIC_PSO_TYPE type);

  private:
    Renderer();
    Renderer(const Renderer &);
    Renderer &operator=(const Renderer &);

    static Renderer *instance_;

    /* PSO */
    GraphicsPSO lighting_pso_;
    GraphicsPSO simple_pso_;

    GraphicsPSO lighting_wire_pso_;

    /* shader */
    ShaderPtr lighting_vs_;
    ShaderPtr lighting_fs_;

    ShaderPtr simple_vs_;
    ShaderPtr simple_fs_;

    /* program */
    ProgramPtr lighting_program_;
    ProgramPtr simple_program_;

    /* framebuffer */
    FramebufferPtr main_framebuffer_;
};

#endif
