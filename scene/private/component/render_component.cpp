#include "render_component.h"
#include "renderer.h"
#include "transform_component.h"

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
