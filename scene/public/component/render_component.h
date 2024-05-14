#ifndef INCLUDED_RENDER_COMPONENT_H
#define INCLUDED_RENDER_COMPONENT_H

#include "component.h"
#include "pso.h"

class RenderComponent : public Component
{
  public:
    RenderComponent(ComponentType type);
    ~RenderComponent();

    GraphicsPSO pso()
    {
        return pso_;
    }
    void set_pso(const GraphicsPSO &pso)
    {
        pso_ = pso;
    }

  private:
    RenderComponent(const RenderComponent &);
    RenderComponent &operator=(const RenderComponent &);

    GraphicsPSO pso_;
};

#endif
