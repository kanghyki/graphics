#include "camera_component.h"
#include "actor.h"
#include "camera_manager.h"
#include "level_manager.h"
#include "light_component.h"
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
    shadow_lights_.clear();
    models_.clear();
    skymaps_.clear();

    camera_.aspect_ = Renderer::GetInstance()->aspect();
    CameraManager::GetInstance()->SetMainCamera(this);
    std::vector<Actor *> actors = LevelManager::GetInstance()->GetCurrentLevel()->GetActors();
    for (const auto &actor : actors)
    {
        if (actor->GetLightComponent() && actor->GetLightComponent()->use_shadow())
        {
            shadow_lights_.push_back(actor);
        }
        if (actor->GetSkyboxComponent())
        {
            skymaps_.push_back(actor);
        }
        if (actor->GetModelComponent())
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

    // RenderShadowMap();
    RenderShading();
    RenderSkybox();
}

void CameraComponent::RenderShadowMap()
{
    // render shadowmap
    for (Actor *actor : shadow_lights_)
    {
        actor->GetLightComponent()->depth_map()->Bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, actor->GetLightComponent()->depth_map()->texture()->width(),
                   actor->GetLightComponent()->depth_map()->texture()->height());
        GraphicsPSOPtr shadow_pso = Renderer::GetInstance()->GetPSO(PsoType::DEPTH_MAP);
        Renderer::GetInstance()->ApplyPSO(shadow_pso);
        auto rm = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        auto light_position = actor->GetTransformComponent()->transform().position_;
        auto light_direction = actor->GetTransformComponent()->transform().direction();
        auto light_view = glm::lookAt(light_position, light_position + light_direction,
                                      glm::vec3(glm::vec4(light_direction, 0.0f) * rm));
        glm::mat4 light_projection;
        if (actor->GetLightComponent()->light().type_ == LightType::DIRECTIONAL)
        {
            light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
        }
        else
        {
            light_projection = glm::perspective(glm::radians((actor->GetLightComponent()->light().falloff_start_ +
                                                              actor->GetLightComponent()->light().falloff_end_) *
                                                             2.0f),
                                                1.0f, 1.0f, 20.0f);
        }
        shadow_pso->program_->Use();
        shadow_pso->program_->SetUniform("view", light_view);
        shadow_pso->program_->SetUniform("proj", light_projection);

        for (Actor *m : models_)
        {
            m->Render(shadow_pso->program_);
        }
    }
    glViewport(0, 0, Renderer::GetInstance()->width(), Renderer::GetInstance()->height());
}

void CameraComponent::RenderShading()
{
    Renderer::GetInstance()->GetFramebuffer(FramebufferType::G_BUFFER)->Bind();
    GraphicsPSOPtr g_buffer_pso = Renderer::GetInstance()->GetPSO(PsoType::G_BUFFER);
    Renderer::GetInstance()->ApplyPSO(g_buffer_pso);
    g_buffer_pso->program_->Use();
    for (Actor *actor : models_)
    {
        actor->Render(g_buffer_pso->program_);
    }
    glUseProgram(0);

    Renderer::GetInstance()->RenderSSAO();
    Renderer::GetInstance()->RenderDeffered();
}

void CameraComponent::RenderSkybox()
{
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