#include "light_component.h"
#include "light_manager.h"
#include "transform_component.h"

LightComponent::LightComponent() : Component(ComponentType::LIGHT)
{
}

LightComponent::~LightComponent()
{
}

void LightComponent::Tick()
{
    LightManager::GetInstance()->AddLight(this);
}

glm::vec3 LightComponent::position() const
{
    return GetTransformComponent()->position();
}

glm::vec3 LightComponent::direction() const
{
    return GetTransformComponent()->CalcRotateMatrix() * glm::vec4(0.0, 0.0f, -1.0f, 0.0f);
}