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

    void set_cube_texture(CubeTexturePtr cube_texture)
    {
        cube_texture_ = cube_texture;
    }

  private:
    SkyboxComponent(const SkyboxComponent &rhs);
    SkyboxComponent &operator=(const SkyboxComponent &rhs);

    CubeTexturePtr cube_texture_;
};

#endif