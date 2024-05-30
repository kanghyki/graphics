#include "component.h"
#include "actor.h"

Component::Component(ComponentType type) : type_(type), owner_(nullptr)
{
}

Component::~Component()
{
}

void Component::Tick()
{
}

void Component::FinalTick()
{
}

OWNER_COMPONENT_GETTER_IMPL(TransformComponent)
OWNER_COMPONENT_GETTER_IMPL(CameraComponent)
OWNER_COMPONENT_GETTER_IMPL(LightComponent)
OWNER_COMPONENT_GETTER_IMPL(ModelComponent)
OWNER_COMPONENT_GETTER_IMPL(TerrainComponent)