#ifndef INCLUDED_MESH_COMPONENT_H
#define INCLUDED_MESH_COMPONENT_H

#include "model.h"
#include "render_component.h"

class ModelComponent : public RenderComponent
{
  public:
    ModelComponent();
    ~ModelComponent();

    void Render(const Program *program) override;

    ModelPtr model()
    {
        return model_;
    }
    void set_model(ModelPtr model)
    {
        model_ = model;
    }

  private:
    ModelComponent(const ModelComponent &);
    ModelComponent &operator=(const ModelComponent &);

    ModelPtr model_;
};

#endif
