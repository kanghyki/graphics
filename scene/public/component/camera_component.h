#ifndef INCLUDED_CAMERA_COMPONENT_H
#define INCLUDED_CAMERA_COMPONENT_H

#include "camera.h"
#include "component.h"
#include "libmath.h"
#include "pso.h"
#include <vector>

class Actor;
class CameraComponent : public Component
{
  public:
    CameraComponent();
    ~CameraComponent();

    void Tick() override;
    void FinalTick() override;
    void Render() override;

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
