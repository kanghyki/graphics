#ifndef INCLUDED_SKYBOX_COMPONENT_H
#define INCLUDED_SKYBOX_COMPONENT_H

#include "render_component.h"
#include "texture.h"

class SkyboxComponent : public RenderComponent
{
  public:
    SkyboxComponent();
    ~SkyboxComponent();

    void Render(const Program *program) override;

    void set_cube_texture(CubeTexturePtr cube_texture)
    {
        cube_texture_ = cube_texture;
    }

    bool activate() const
    {
        return activate_;
    }
    void set_activate(bool activate)
    {
        activate_ = activate;
    }

  private:
    SkyboxComponent(const SkyboxComponent &rhs);
    SkyboxComponent &operator=(const SkyboxComponent &rhs);

    CubeTexturePtr cube_texture_;
    bool activate_{true};
};

#endif