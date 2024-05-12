#ifndef INCLUDED_TRANSFORM_COMPONENT_H
#define INCLUDED_TRANSFORM_COMPONENT_H

#include "component.h"
#include "libmath.h"

class TransformComponent : public Component
{
  public:
    TransformComponent();
    ~TransformComponent();

    void Tick() override;

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
    TransformComponent(const TransformComponent &);
    TransformComponent &operator=(const TransformComponent &);

    glm::vec3 position_;
    glm::vec3 scale_;
};

#endif
