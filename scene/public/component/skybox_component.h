#ifndef INCLUDED_SKYBOX_COMPONENT_H
#define INCLUDED_SKYBOX_COMPONENT_H

#include "render_component.h"
#include "texture.h"

class SkyboxComponent : public RenderComponent
{
  public:
    SkyboxComponent();
    ~SkyboxComponent();

    void Render(ProgramPtr program) override;

  private:
    SkyboxComponent(const SkyboxComponent &rhs);
    SkyboxComponent &operator=(const SkyboxComponent &rhs);
};

#endif