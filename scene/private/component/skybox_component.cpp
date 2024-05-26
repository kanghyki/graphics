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
    glActiveTexture(GL_TEXTURE0);
    cube_texture_->Bind();
    program->SetUniform("cubemap", 0);
    Renderer::GetInstance()->box_mesh()->Draw(program.get());
}