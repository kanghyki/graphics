#include "transform_component.h"

TransformComponent::TransformComponent()
    : Component(ComponentType::TRANSFORM), position_(0.0f, 0.0f, 0.0f), scale_(1.0f, 1.0f, 1.0f)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Tick()
{
}
