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
    light_.transform_ = GetTransformComponent()->transform();

    LightManager::GetInstance()->AddLight(light_.ToData());
}
