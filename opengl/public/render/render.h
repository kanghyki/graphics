#ifndef INCLUDED_RENDER_H
#define INCLUDED_RENDER_H

#include "buffer.h"
#include "framebuffer.h"
#include "program.h"
#include "ptr.h"
#include "render_params.h"

CLASS_PTR(Render);
class Render
{
  public:
    Render();
    virtual ~Render();

    void Use(const RenderParams &params);

    virtual void SetUniform(const RenderParams &params) = 0;

  protected:
    virtual void Setup() = 0;
    virtual void AfterFramebufferBind() = 0;
    virtual void AfterProgramUse() = 0;

  private:
    Render(const Render &);
    Render &operator=(const Render &);

    FramebufferPtr framebuffer_;
    ProgramPtr program_;
    BufferPtr uniform_buffer_;
};

#endif
