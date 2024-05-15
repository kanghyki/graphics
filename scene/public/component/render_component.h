#ifndef INCLUDED_RENDER_COMPONENT_H
#define INCLUDED_RENDER_COMPONENT_H

#include "component.h"
#include "model.h"

class RenderComponent : public Component
{
  public:
    RenderComponent(ComponentType type);
    virtual ~RenderComponent();

    void Render() override;

    ModelPtr model()
    {
        return model_;
    }
    void set_model(const ModelPtr &model)
    {
        model_ = model;
    }

  private:
    RenderComponent(const RenderComponent &);
    RenderComponent &operator=(const RenderComponent &);

    ModelPtr model_;
};

#endif
