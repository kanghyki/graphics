#include "actor.h"
#include "component.h"

uint32_t Actor::s_id_ = 1;

Actor ::Actor(const std::string &name) : id_(s_id_++), name_(name), components_{nullptr}, render_component_(nullptr)
{
}

Actor ::~Actor()
{
}

void Actor::Tick()
{
    for (int i = 0; i < static_cast<int>(ComponentType::EOL); ++i)
    {
        if (components_[i])
        {
            components_[i]->Tick();
        }
    }
}

void Actor::Render()
{
    if (render_component_)
    {
        render_component_->Render();
    }
}

void Actor::SetComponent(Component *component)
{
    component->owner_ = this;
    components_[static_cast<int>(component->type())] = component;
    switch (component->type())
    {
    case ComponentType::MESH:
    case ComponentType::SKY_BOX:
    case ComponentType::LAND_SCAPE:
        render_component_ = component;
        break;
    default:
        break;
    }
}

Component *Actor::GetComponent(ComponentType type)
{
    return components_[static_cast<int>(type)];
}
