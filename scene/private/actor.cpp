#include "actor.h"
#include "component.h"
#include "render_component.h"

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

void Actor::FinalTick()
{
    for (int i = 0; i < static_cast<int>(ComponentType::EOL); ++i)
    {
        if (components_[i])
        {
            components_[i]->FinalTick();
        }
    }
}

void Actor::Render(ProgramPtr program)
{
    if (render_component_)
    {
        render_component_->Render(program);
    }
}

void Actor::SetComponent(std::shared_ptr<Component> component)
{
    component->owner_ = this;
    components_[static_cast<int>(component->type())] = component;
    switch (component->type())
    {
    case ComponentType::MODEL:
    case ComponentType::SKYBOX:
        render_component_ = std::static_pointer_cast<RenderComponent>(component);
        break;
    default:
        break;
    }
}

void Actor::RemoveComponent(ComponentType type)
{
    if (type == ComponentType::TRANSFORM)
    {
        if (GetCameraComponent() || GetLightComponent() || GetModelComponent())
        {
            return;
        }
    }
    if (type == ComponentType::MODEL)
    {

        render_component_ = nullptr;
    }
    components_[static_cast<int>(type)] = nullptr;
}

std::shared_ptr<Component> Actor::GetComponent(ComponentType type)
{
    return components_[static_cast<int>(type)];
}

void Actor::AddCameraComponent()
{
    if (!components_[static_cast<int>(ComponentType::TRANSFORM)])
    {
        Actor::AddTransformComponent();
    }
    SetComponent(ComponentFactory().Generate(ComponentType::CAMERA));
}

void Actor::AddTransformComponent()
{
    SetComponent(ComponentFactory().Generate(ComponentType::TRANSFORM));
}

void Actor::AddLightComponent()
{
    if (!components_[static_cast<int>(ComponentType::TRANSFORM)])
    {
        Actor::AddTransformComponent();
    }
    SetComponent(ComponentFactory().Generate(ComponentType::LIGHT));
}

void Actor::AddModelComponent()
{
    if (!components_[static_cast<int>(ComponentType::TRANSFORM)])
    {
        Actor::AddTransformComponent();
    }
    SetComponent(ComponentFactory().Generate(ComponentType::MODEL));
}

void Actor::AddSkyboxComponent()
{
    if (!components_[static_cast<int>(ComponentType::TRANSFORM)])
    {
        Actor::AddTransformComponent();
    }
    SetComponent(ComponentFactory().Generate(ComponentType::SKYBOX));
}

COMPONENT_GETTER_IMPL(TRANSFORM, TransformComponent)
COMPONENT_GETTER_IMPL(CAMERA, CameraComponent)
COMPONENT_GETTER_IMPL(LIGHT, LightComponent)
COMPONENT_GETTER_IMPL(MODEL, ModelComponent)
COMPONENT_GETTER_IMPL(SKYBOX, SkyboxComponent)