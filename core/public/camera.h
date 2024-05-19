#ifndef INCLUDED_CAMERA_H
#define INCLUDED_CAMERA_H

#include "libmath.h"
#include "transform.h"

class Camera
{
  public:
    Camera();
    ~Camera();

    void set_transform(const Transform &transform)
    {
        transform_ = transform;
    }
    glm::mat4 CalcViewMatrix();
    glm::mat4 CalcPerspectiveProjectionMatrix();

    float fov_y_;
    float aspect_;
    float near_plane_;
    float far_plane_;

  private:
    Camera(const Camera &);
    Camera &operator=(const Camera &);

    Transform transform_;

    glm::vec3 up_;
};

#endif
