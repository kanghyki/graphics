#ifndef INCLUDED_LIGHTING_RENDER_H
#define INCLUDED_LIGHTING_RENDER_H

#include "ptr.h"
#include "render.h"

CLASS_PTR(LightingRender);
class LightingRender : public Render
{
  public:
    static LightingRenderPtr Create();
    ~LightingRender();

    virtual void SetUniform(const RenderParams &params) override;

  private:
    LightingRender();
    LightingRender(const LightingRender &);
    LightingRender &operator=(const LightingRender &);

    virtual void Setup() override;
    virtual void AfterFramebufferBind() override;
    virtual void AfterProgramUse() override;
};

#endif
