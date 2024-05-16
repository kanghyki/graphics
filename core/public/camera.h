#ifndef INCLUDED_CAMERA_H
#define INCLUDED_CAMERA_H

#include "libmath.h"
#include "transform.h"

class Camera
{
  public:
    Camera();
    ~Camera();

    void set_transform(Transform transform)
    {
        transform_ = transform;
    }
    glm::mat4 CalcViewMatrix();
    glm::mat4 CalcPerspectiveProjectionMatrix();

  private:
    Camera(const Camera &);
    Camera &operator=(const Camera &);

    Transform transform_;

    // float pitch_;
    // float yaw_;
    float fov_y_;
    float aspect_;
    float near_plane_;
    float far_plane_;

    glm::vec3 up_;
    // mutable glm::vec3 front_;
};

#endif
