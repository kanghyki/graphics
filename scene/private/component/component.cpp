#include "component.h"

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

void Component::Render()
{
}
