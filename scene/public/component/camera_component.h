#ifndef INCLUDED_CAMERA_COMPONENT_H
#define INCLUDED_CAMERA_COMPONENT_H

#include "camera.h"
#include "component.h"
#include "framebuffer.h"
#include "graphics_pso.h"
#include "libmath.h"
#include <utility>
#include <vector>

class NoInherit
{
  private:
    NoInherit()
    {
    }
    virtual ~NoInherit()
    {
    }

    friend CameraComponent;
};

class Actor;
class CameraComponent : public Component, private NoInherit
{
  public:
    CameraComponent();
    ~CameraComponent();

    void Tick() override;
    void FinalTick() override;
    void Render();

    Camera &camera()
    {
        return camera_;
    }
    const Camera &camera() const
    {
        return camera_;
    }

    void set_render(const std::pair<GraphicsPSOPtr, FramebufferPtr> &render)
    {
        render_ = render;
    }

  private:
    CameraComponent(const CameraComponent &);
    CameraComponent &operator=(const CameraComponent &);

    Camera camera_;

    void RenderShadowMap();
    void RenderSkybox();
    void RenderShading();

    // uint32_t layer_mask_;
    std::vector<Actor *> shadow_lights_;
    std::vector<Actor *> skymaps_;
    std::vector<Actor *> models_;
    std::pair<GraphicsPSOPtr, FramebufferPtr> render_;
};

#endif
