#ifndef INCLUDED_TRANSFORM_H
#define INCLUDED_TRANSFORM_H

#include "libmath.h"

class Transform
{
  public:
    Transform();
    Transform(const Transform &t2);
    Transform &operator=(const Transform &t2);
    ~Transform();

    glm::vec3 position()
    {
        return position_;
    }
    void set_position(const glm::vec3 &position)
    {
        position_ = position;
    }
    glm::vec3 scale()
    {
        return scale_;
    }
    void set_scale(const glm::vec3 &scale)
    {
        scale_ = scale;
    }
    glm::vec3 rotation()
    {
        return rotation_;
    }
    void set_rotation(const glm::vec3 &rotation)
    {
        rotation_ = rotation;
    }

  private:
    glm::vec3 position_;
    glm::vec3 scale_;
    glm::vec3 rotation_;
};

#endif
