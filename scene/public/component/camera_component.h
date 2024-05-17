#ifndef INCLUDED_CAMERA_COMPONENT_H
#define INCLUDED_CAMERA_COMPONENT_H

#include "camera.h"
#include "component.h"
#include "graphics_pso.h"
#include "libmath.h"
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

    void set_pso(const GraphicsPSO &pso)
    {
        pso_ = pso;
    }

  private:
    CameraComponent(const CameraComponent &);
    CameraComponent &operator=(const CameraComponent &);

    Camera camera_;

    // uint32_t layer_mask_;
    std::vector<Actor *> actors_;
    GraphicsPSO pso_;
};

#endif
