#ifndef INCLUDED_TRANSFORM_COMPONENT_H
#define INCLUDED_TRANSFORM_COMPONENT_H

#include "component.h"
#include "libmath.h"
#include "transform.h"

class TransformComponent : public Component
{
  public:
    TransformComponent();
    ~TransformComponent();

    void Tick() override;

    glm::vec3 position() const
    {
        return transform_.position();
    }
    void set_position(const glm::vec3 &position)
    {
        transform_.set_position(position);
    }

    glm::vec3 scale() const
    {
        return transform_.scale();
    }
    void set_scale(const glm::vec3 &scale)
    {
        transform_.set_scale(scale);
    }

    glm::vec3 rotation() const
    {
        return transform_.rotation();
    }
    void set_rotation(const glm::vec3 &rotation)
    {
        transform_.set_rotation(rotation);
    }

    glm::mat4 CalcModelMatrix() const
    {
        return transform_.CalcModelMatrix();
    }
    glm::mat4 CalcScaleMatrix() const
    {
        return transform_.CalcScaleMatrix();
    }
    glm::mat4 CalcRotateMatrix() const
    {
        return transform_.CalcRotateMatrix();
    }
    glm::mat4 CalcTranslateMatrix() const
    {
        return transform_.CalcTranslateMatrix();
    }

  private:
    TransformComponent(const TransformComponent &);
    TransformComponent &operator=(const TransformComponent &);

    Transform transform_;
};

#endif
