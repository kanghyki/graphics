#include "transform.h"

Transform::Transform() : position_(0.0f, 0.0f, 0.0f), scale_(1.0f, 1.0f, 1.0f)
{
}

Transform::Transform(const Transform &t2)
{
    position_ = t2.position_;
    scale_ = t2.scale_;
    rotation_ = t2.rotation_;
}

Transform &Transform::operator=(const Transform &t2)
{
    if (this != &t2)
    {
        position_ = t2.position_;
        scale_ = t2.scale_;
        rotation_ = t2.rotation_;
    }
    return *this;
}

Transform::~Transform()
{
}
