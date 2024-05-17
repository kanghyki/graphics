#include "light_component.h"
#include "transform_component.h"
#include <spdlog/spdlog.h>

LightComponent::LightComponent() : Component(ComponentType::LIGHT)
{
}

LightComponent::~LightComponent()
{
}

void LightComponent::Tick()
{
    light_.transform_ = GetTransformComponent()->transform();
}