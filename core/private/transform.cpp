#include "transform.h"

Transform::Transform() : position_(0.0f), scale_(1.0f), rotation_(0.0f)
{
}

Transform::Transform(const Transform &t)
{
    position_ = t.position_;
    scale_ = t.scale_;
    rotation_ = t.rotation_;
}

Transform &Transform::operator=(const Transform &t)
{
    if (this != &t)
    {
        position_ = t.position_;
        scale_ = t.scale_;
        rotation_ = t.rotation_;
    }
    return *this;
}

Transform::~Transform()
{
}

glm::mat4 Transform::CalcModelMatrix() const
{
    return CalcTranslateMatrix() * CalcRotateMatrix() * CalcScaleMatrix();
}
glm::mat4 Transform::CalcScaleMatrix() const
{
    return glm::scale(glm::mat4(1.0f), scale_);
}
glm::mat4 Transform::CalcRotateMatrix() const
{
    return glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
           glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
           glm::rotate(glm::mat4(1.0f), glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));
}
glm::mat4 Transform::CalcTranslateMatrix() const
{
    return glm::translate(glm::mat4(1.0f), position_);
}