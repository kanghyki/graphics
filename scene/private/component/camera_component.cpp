#include "camera_component.h"
#include "actor.h"
#include "camera_manager.h"
#include "level_manager.h"
#include "renderer.h"
#include "transform_component.h"

CameraComponent::CameraComponent() : Component(ComponentType::CAMERA)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Tick()
{
    models_.clear();
    skymaps_.clear();

    camera_.aspect_ = Renderer::GetInstance()->aspect();
    CameraManager::GetInstance()->SetMainCamera(this);
    std::vector<Actor *> actors = LevelManager::GetInstance()->GetCurrentLevel()->GetActors();
    for (const auto &actor : actors)
    {
        if (actor->GetSkyboxComponent())
        {
            skymaps_.push_back(actor);
        }
        else if (actor->GetModelComponent())
        {
            models_.push_back(actor);
        }
    }
}

void CameraComponent::FinalTick()
{
    camera_.set_transform(GetTransformComponent()->transform());
}

void CameraComponent::Render()
{
    Renderer::GetInstance()->GetUBO(UBOType::CAMERA)->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraUniform, c_view_position), sizeof(glm::vec3),
                    glm::value_ptr(camera_.transform_.position_));

    Renderer::GetInstance()->GetUBO(UBOType::MATRICES)->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_view), sizeof(glm::mat4),
                    glm::value_ptr(camera_.CalcViewMatrix()));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_proj), sizeof(glm::mat4),
                    glm::value_ptr(camera_.CalcPerspectiveProjectionMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    Renderer::GetInstance()->GetFramebuffer(FramebufferType::G_BUFFER)->Bind();
    GraphicsPSOPtr g_buffer_pso = Renderer::GetInstance()->GetPSO(PsoType::G_BUFFER);
    Renderer::GetInstance()->ApplyPSO(g_buffer_pso);
    g_buffer_pso->program_->Use();
    for (Actor *actor : models_)
    {
        actor->Render(g_buffer_pso->program_);
    }
    glUseProgram(0);

    Renderer::GetInstance()->RenderDeffered();

    Renderer::GetInstance()->GetFramebuffer(FramebufferType::MAIN)->Bind();
    GraphicsPSOPtr skybox_pso = Renderer::GetInstance()->GetPSO(PsoType::SKYBOX);
    Renderer::GetInstance()->ApplyPSO(skybox_pso);
    skybox_pso->program_->Use();
    for (Actor *actor : skymaps_)
    {
        actor->Render(skybox_pso->program_);
    }
    glUseProgram(0);
}
