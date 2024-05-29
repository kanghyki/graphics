#ifndef INCLUDED_RENDER_COMPONENT_H
#define INCLUDED_RENDER_COMPONENT_H

#include "component.h"
#include "program.h"

class RenderComponent : public Component
{
  public:
    RenderComponent(ComponentType type);
    virtual ~RenderComponent();

    virtual void Render(const Program *program) = 0;

  private:
    RenderComponent(const RenderComponent &);
    RenderComponent &operator=(const RenderComponent &);
};

#endif
