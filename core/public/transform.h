#ifndef INCLUDED_TRANSFORM_H
#define INCLUDED_TRANSFORM_H

#include "libmath.h"

class Transform
{
  public:
    Transform();
    Transform(const Transform &t);
    Transform &operator=(const Transform &t);
    ~Transform();

    glm::mat4 CalcModelMatrix() const;
    glm::mat4 CalcScaleMatrix() const;
    glm::mat4 CalcRotateMatrix() const;
    glm::mat4 CalcTranslateMatrix() const;

    glm::vec3 position() const
    {
        return position_;
    }
    void set_position(const glm::vec3 &position)
    {
        position_ = position;
    }

    glm::vec3 scale() const
    {
        return scale_;
    }
    void set_scale(const glm::vec3 &scale)
    {
        scale_ = scale;
    }

    glm::vec3 rotation() const
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
