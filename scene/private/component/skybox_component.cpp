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
    static auto cubeRight = Image::Load("../../asset/cube_texture/0/right.jpg", false);
    static auto cubeLeft = Image::Load("../../asset/cube_texture/0/left.jpg", false);
    static auto cubeTop = Image::Load("../../asset/cube_texture/0/top.jpg", false);
    static auto cubeBottom = Image::Load("../../asset/cube_texture/0/bottom.jpg", false);
    static auto cubeFront = Image::Load("../../asset/cube_texture/0/front.jpg", false);
    static auto cubeBack = Image::Load("../../asset/cube_texture/0/back.jpg", false);
    static auto cube_texture_ = CubeTexture::Create({
        cubeRight.get(),
        cubeLeft.get(),
        cubeTop.get(),
        cubeBottom.get(),
        cubeFront.get(),
        cubeBack.get(),
    });
    static auto box = Mesh::CreateBox();
    static auto model = glm::scale(glm::mat4(1.0), glm::vec3(200.0f));
    glActiveTexture(GL_TEXTURE0);
    cube_texture_->Bind();
    program->SetUniform("cubemap", 0);
    program->SetUniform("model", model);
    box->Draw(program.get());
}