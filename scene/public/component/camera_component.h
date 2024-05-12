#ifndef INCLUDED_CAMERA_COMPONENT_H
#define INCLUDED_CAMERA_COMPONENT_H

#include "component.h"
#include "libmath.h"

class CameraComponent : public Component
{
  public:
    CameraComponent();
    ~CameraComponent();

    void Tick() override;
    void Render() override;

  private:
    CameraComponent(const CameraComponent &);
    CameraComponent &operator=(const CameraComponent &);

    glm::mat4 CalcViewMatrix();
    glm::mat4 CalcPerspectiveProjectionMatrix();

    float pitch_;
    float yaw_;
    float fov_y_;
    float aspect_;
    float near_plane_;
    float far_plane_;

    glm::vec3 up_;
    mutable glm::vec3 front_;
};

#endif
