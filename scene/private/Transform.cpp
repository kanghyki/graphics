#include "Transform.h"

Transform ::Transform() : position_(0.0f), scale_(1.0f)
{
}

Transform ::~Transform()
{
}

Transform ::Transform(const Transform &c)
{
    position_ = c.position_;
    scale_ = c.scale_;
}

Transform &Transform::operator=(const Transform &c)
{
    if (&c != this)
    {
        position_ = c.position_;
        scale_ = c.scale_;
    }
    return *this;
}
