#include "light_component.h"
#include "opengl_device.h"
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
    light_.ToUniform().Sub();
}
