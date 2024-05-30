#include "camera_component.h"
#include "actor.h"
#include "camera_manager.h"
#include "level_manager.h"
#include "libmath.h"
#include "light_component.h"
#include "light_manager.h"
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
    terrians_.clear();

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
        if (actor->GetTerrainComponent())
        {
            terrians_.push_back(actor);
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

    RenderTerrain();
    RenderShadowMap();
    RenderShading();
    RenderSkybox();
}

void CameraComponent::RenderTerrain()
{
    Renderer::GetInstance()->GetFramebuffer(FramebufferType::G_BUFFER)->Bind();
    GraphicsPSO *pso = Renderer::GetInstance()->GetPSO(PsoType::TERRAIN);
    Renderer::GetInstance()->ApplyPSO(pso);
    pso->program_->Use();
    for (Actor *actor : terrians_)
    {
        actor->Render(pso->program_.get());
    }
    glUseProgram(0);
}

void CameraComponent::RenderShadowMap()
{
    glViewport(0, 0, 1280, 1280);
    for (Actor *actor : shadow_lights_)
    {
        LightComponent *lc = actor->GetLightComponent();
        if (lc->type() == LightType::DIRECTIONAL)
        {
            RenderDirectionalShadowMap(lc);
        }
        else if (lc->type() == LightType::POINT)
        {
            RenderPointShadowMap(lc);
        }
        else if (lc->type() == LightType::SPOT)
        {
            RenderSpotShadowMap(lc);
        }
    }
    glViewport(0, 0, Renderer::GetInstance()->width(), Renderer::GetInstance()->height());
    LightManager::GetInstance()->UpdateUBO();
}

void CameraComponent::RenderSpotShadowMap(LightComponent *lc)
{
    lc->depth_map()->Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    GraphicsPSO *pso = Renderer::GetInstance()->GetPSO(PsoType::DEPTH_MAP);
    Renderer::GetInstance()->ApplyPSO(pso);

    pso->program_->SetUniform("view", lc->CalcView());
    pso->program_->SetUniform("proj", lc->CalcProj());
    for (Actor *model : models_)
    {
        if (!model->GetLightComponent())
        {
            pso->program_->SetUniform("model", model->GetTransformComponent()->CalcModelMatrix());
            model->Render(pso->program_.get());
        }
    }
    glUseProgram(0);
}

void CameraComponent::RenderPointShadowMap(LightComponent *lc)
{
    lc->depth_map()->Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    GraphicsPSO *pso = Renderer::GetInstance()->GetPSO(PsoType::OMNI_DEPTH_MAP);
    Renderer::GetInstance()->ApplyPSO(pso);

    glm::vec3 pos = lc->position();
    glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, lc->near_plane(), lc->far_plane());
    std::vector<glm::mat4> transform;
    transform.reserve(6);
    transform.push_back(proj * glm::lookAt(pos, pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    transform.push_back(proj * glm::lookAt(pos, pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    transform.push_back(proj * glm::lookAt(pos, pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    transform.push_back(proj * glm::lookAt(pos, pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    transform.push_back(proj * glm::lookAt(pos, pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    transform.push_back(proj * glm::lookAt(pos, pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

    pso->program_->SetUniform("light_transform", transform);
    pso->program_->SetUniform("far_plane", lc->far_plane());
    pso->program_->SetUniform("light_position", lc->position());
    for (Actor *model : models_)
    {
        if (!model->GetLightComponent())
        {
            pso->program_->SetUniform("model", model->GetTransformComponent()->CalcModelMatrix());
            model->Render(pso->program_.get());
        }
    }
    glUseProgram(0);
}

void CameraComponent::RenderDirectionalShadowMap(LightComponent *lc)
{
    lc->depth_map()->Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    GraphicsPSO *pso = Renderer::GetInstance()->GetPSO(PsoType::CASCADE_SHADOW_MAP);
    Renderer::GetInstance()->ApplyPSO(pso);

    auto matrices = lc->CalcLightPVMatrices(fov_y(), aspect(), near_plane(), far_plane(), CalcViewMatrix());
    pso->program_->SetUniform("light_matrices", matrices);

    for (Actor *model : models_)
    {
        if (!model->GetLightComponent())
        {
            pso->program_->SetUniform("model", model->GetTransformComponent()->CalcModelMatrix());
            model->Render(pso->program_.get());
        }
    }
    glUseProgram(0);
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

    int empty = pso->program_->ActivateTexture(DepthMap::empty()->texture());
    for (int i = 0; i < LightsUniform::SHADOW_2D_MAX; ++i)
    {
        const std::string &name = fmt::format("l_shadow[{}]", i);
        pso->program_->SetUniform(name, empty);
    }
    int empty_cube = pso->program_->ActivateTexture(DepthMap::empty_cube()->texture());
    for (int i = 0; i < LightsUniform::SHADOW_CUBE_MAX; ++i)
    {
        const std::string &name = fmt::format("l_shadow_cube[{}]", i);
        pso->program_->SetUniform(name, empty_cube);
    }

    for (Actor *shadow_light : shadow_lights_)
    {
        LightComponent *lc = shadow_light->GetLightComponent();
        if (lc->type() == LightType::DIRECTIONAL)
        {
            pso->program_->ActivateTexture("l_shadow_array", lc->depth_map()->texture());
        }
        else if (lc->type() == LightType::POINT)
        {
            const std::string &name = fmt::format("l_shadow_cube[{}]", lc->shadow_id());
            pso->program_->ActivateTexture(name, lc->depth_map()->texture());
        }
        else if (lc->type() == LightType::SPOT)
        {
            const std::string &name = fmt::format("l_shadow[{}]", lc->shadow_id());
            pso->program_->ActivateTexture(name, lc->depth_map()->texture());
        }
    }
    Renderer::GetInstance()->plane_mesh()->Draw(nullptr);
    glUseProgram(0);

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
