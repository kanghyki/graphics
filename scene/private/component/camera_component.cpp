#include "camera_component.h"
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
    CameraManager::GetInstance()->RegisterCamera(this);
    actors_ = LevelManager::GetInstance()->GetCurrentLevel()->GetActors();
}

void CameraComponent::FinalTick()
{
    camera_.set_transform(GetTransformComponent()->transform());
}

void CameraComponent::Render()
{
    Renderer::GetInstance()->set_pso(pso_);
    Renderer::GetInstance()->set_cur_camera(&camera_);

    for (Actor *actor : actors_)
    {
        actor->Render();
    }
}
