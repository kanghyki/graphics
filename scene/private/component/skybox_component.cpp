#include "skybox_component.h"
#include "renderer.h"
#include "transform_component.h"

SkyboxComponent::SkyboxComponent() : RenderComponent(ComponentType::SKYBOX)
{
}

SkyboxComponent::~SkyboxComponent()
{
}

void SkyboxComponent::Render(const Program *program)
{
    program->ActivateTexture("cubemap", cube_texture_.get());
    Renderer::GetInstance()->box_mesh()->Draw(program);
}