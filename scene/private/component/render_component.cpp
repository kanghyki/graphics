#include "render_component.h"
#include "renderer.h"

RenderComponent::RenderComponent(ComponentType type) : Component(type)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Render()
{
    Renderer::GetInstance()->Draw(model());
}
