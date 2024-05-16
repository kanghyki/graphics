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

    void set_transform(const Transform &t)
    {
        transform_ = t;
    }
    const Transform &transform() const
    {
        return transform_;
    }
    Transform &transform()
    {
        return transform_;
    }

  private:
    TransformComponent(const TransformComponent &);
    TransformComponent &operator=(const TransformComponent &);

    Transform transform_;
};

#endif
