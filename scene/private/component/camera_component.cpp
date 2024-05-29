#include "camera_component.h"
#include "actor.h"
#include "camera_manager.h"
#include "level_manager.h"
#include "light_component.h"
#include "renderer.h"
#include "transform_component.h"
#include <spdlog/spdlog.h>

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

    aspect_ = Renderer::GetInstance()->aspect();
    CameraManager::GetInstance()->SetMainCamera(this);
    std::vector<Actor *> actors = LevelManager::GetInstance()->GetCurrentLevel()->GetActors();
    for (const auto &actor : actors)
    {
        if (actor->GetLightComponent() && (actor->GetLightComponent()->use_shadow()))
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
}

void CameraComponent::Render()
{
    Renderer::GetInstance()->GetUBO(UBOType::CAMERA)->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraUniform, c_view_position), sizeof(glm::vec3),
                    glm::value_ptr(GetTransformComponent()->position()));

    Renderer::GetInstance()->GetUBO(UBOType::MATRICES)->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_view), sizeof(glm::mat4),
                    glm::value_ptr(CalcViewMatrix()));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_proj), sizeof(glm::mat4),
                    glm::value_ptr(CalcPerspectiveProjectionMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    RenderShadowMap();
    RenderShading();
    // RenderSkybox();
}

void CameraComponent::RenderShadowMap()
{
    glViewport(0, 0, 1280, 1280);
    for (Actor *actor : shadow_lights_)
    {
        actor->GetLightComponent()->depth_map()->Bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        GraphicsPSO *depth_map_pso = Renderer::GetInstance()->GetPSO(PsoType::DEPTH_MAP);
        Renderer::GetInstance()->ApplyPSO(depth_map_pso);

        LightComponent *lc = actor->GetLightComponent();
        int width = lc->depth_map()->texture()->width();
        int height = lc->depth_map()->texture()->height();

        depth_map_pso->program_->SetUniform("view", lc->CalcView());
        depth_map_pso->program_->SetUniform("proj", lc->CalcProj());
        for (Actor *model : models_)
        {
            depth_map_pso->program_->SetUniform("model", model->GetTransformComponent()->CalcModelMatrix());
            model->Render(nullptr);
        }
    }
    glViewport(0, 0, Renderer::GetInstance()->width(), Renderer::GetInstance()->height());
}

void CameraComponent::RenderShading()
{
    Renderer::GetInstance()->GetFramebuffer(FramebufferType::G_BUFFER)->Bind();
    GraphicsPSO *g_buffer_pso = Renderer::GetInstance()->GetPSO(PsoType::G_BUFFER);
    Renderer::GetInstance()->ApplyPSO(g_buffer_pso);
    for (Actor *actor : models_)
    {
        actor->Render(g_buffer_pso->program_.get());
    }
    glUseProgram(0);

    Renderer::GetInstance()->RenderSSAO();

    Framebuffer *main_fb = Renderer::GetInstance()->GetFramebuffer(FramebufferType::MAIN);
    Framebuffer *g_buffer = Renderer::GetInstance()->GetFramebuffer(FramebufferType::G_BUFFER);
    main_fb->Bind();
    GraphicsPSO *pso = Renderer::GetInstance()->GetPSO(PsoType::DEFFERED_SHADING);
    Renderer::GetInstance()->ApplyPSO(pso);
    pso->program_->SetUniform("model", glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
    pso->program_->ActivateTexture("gPosition", g_buffer->color_attachment(0));
    pso->program_->ActivateTexture("gNormal", g_buffer->color_attachment(1));
    pso->program_->ActivateTexture("gAlbedoSpec", g_buffer->color_attachment(2));
    pso->program_->ActivateTexture("gEmissive", g_buffer->color_attachment(3));
    bool use_ssao = Renderer::GetInstance()->use_ssao_;
    pso->program_->SetUniform("use_ssao", use_ssao);
    if (use_ssao)
    {
        pso->program_->ActivateTexture(
            "SSAO", Renderer::GetInstance()->GetFramebuffer(FramebufferType::SSAO)->color_attachment(0));
    }
    for (Actor *shadow_light : shadow_lights_)
    {
        LightComponent *lc = shadow_light->GetLightComponent();
        auto name = fmt::format("l_shadow[{}]", lc->shadow_id());
        pso->program_->ActivateTexture(name, lc->depth_map()->texture());
    }
    Renderer::GetInstance()->plane_mesh()->Draw(nullptr);

    g_buffer->Bind(GL_READ_FRAMEBUFFER);
    main_fb->Bind(GL_DRAW_FRAMEBUFFER);
    int width = Renderer::GetInstance()->width();
    int height = Renderer::GetInstance()->height();
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void CameraComponent::RenderSkybox()
{
    Renderer::GetInstance()->GetFramebuffer(FramebufferType::MAIN)->Bind();
    GraphicsPSO *skybox_pso = Renderer::GetInstance()->GetPSO(PsoType::SKYBOX);
    Renderer::GetInstance()->ApplyPSO(skybox_pso);
    for (Actor *actor : skymaps_)
    {
        actor->Render(skybox_pso->program_.get());
    }
    glUseProgram(0);
}

glm::mat4 CameraComponent::CalcViewMatrix()
{
    TransformComponent *tc = GetTransformComponent();
    glm::vec3 front = glm::rotate(glm::mat4(1.0f), glm::radians(tc->rotation().x), glm::vec3(0.0f, 1.0f, 0.0f)) *
                      glm::rotate(glm::mat4(1.0f), glm::radians(tc->rotation().y), glm::vec3(1.0f, 0.0f, 0.0f)) *
                      glm::vec4(0.0, 0.0, -1.0, 0.0);

    return glm::lookAt(tc->position(), tc->position() + front, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 CameraComponent::CalcPerspectiveProjectionMatrix()
{
    return glm::perspective(glm::radians(fov_y_), aspect_, near_plane_, far_plane_);
}
