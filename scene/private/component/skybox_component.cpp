#include "skybox_component.h"
#include "renderer.h"
#include "transform_component.h"

SkyboxComponent::SkyboxComponent() : RenderComponent(ComponentType::SKYBOX)
{
}

SkyboxComponent::~SkyboxComponent()
{
}

void SkyboxComponent::Render(ProgramPtr program)
{
}