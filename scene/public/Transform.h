#ifndef INCLUDED_TRANSFORM_H
#define INCLUDED_TRANSFORM_H

#include "libmath.h"

class Transform
{
  public:
    Transform();
    ~Transform();
    Transform(const Transform &c);
    Transform &operator=(const Transform &c);

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

  private:
    glm::vec3 position_;
    glm::vec3 scale_;
};

#endif
