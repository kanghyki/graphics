#ifndef INCLUDED_SINGLE_COLOR_RENDER_H
#define INCLUDED_SINGLE_COLOR_RENDER_H

#include "ptr.h"
#include "render.h"

CLASS_PTR(SingleColorRender);
class SingleColorRender : public Render
{
  public:
    static SingleColorRenderPtr Create();
    ~SingleColorRender();

    virtual void SetUniform(const RenderParams &params) override;

  private:
    SingleColorRender();
    SingleColorRender(const SingleColorRender &);
    SingleColorRender &operator=(const SingleColorRender &);

    virtual void Setup() override;
    virtual void AfterFramebufferBind() override;
    virtual void AfterProgramUse() override;
};

#endif
