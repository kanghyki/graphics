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
    glm::vec3 direction() const;

    glm::vec3 position_;
    glm::vec3 scale_;
    glm::vec3 rotation_;
    glm::vec4 direction_;
};

#endif
