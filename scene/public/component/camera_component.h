#ifndef INCLUDED_CAMERA_COMPONENT_H
#define INCLUDED_CAMERA_COMPONENT_H

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

    glm::mat4 CalcViewMatrix();
    glm::mat4 CalcPerspectiveProjectionMatrix();

    float fov_y() const
    {
        return fov_y_;
    }
    void set_fov_y(float fov_y)
    {
        fov_y_ = fov_y;
    }

    float aspect() const
    {
        return aspect_;
    }
    void set_aspect(float aspect)
    {
        aspect_ = aspect;
    }

    float near_plane() const
    {
        return near_plane_;
    }
    void set_near_plane(float near_plane)
    {
        near_plane_ = near_plane;
    }

    float far_plane() const
    {
        return far_plane_;
    }
    void set_far_plane(float far_plane)
    {
        far_plane_ = far_plane;
    }

  private:
    CameraComponent(const CameraComponent &);
    CameraComponent &operator=(const CameraComponent &);

    void RenderShadowMap();
    void RenderSkybox();
    void RenderShading();

    float fov_y_{45.0f};
    float aspect_{1.0f};
    float near_plane_{0.01f};
    float far_plane_{200.0f};

    // uint32_t layer_mask_;
    std::vector<Actor *> shadow_lights_;
    std::vector<Actor *> skymaps_;
    std::vector<Actor *> models_;
};

#endif
