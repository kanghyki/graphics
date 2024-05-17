#include "camera_component.h"
#include "actor.h"
#include "camera_manager.h"
#include "level_manager.h"
#include "opengl_device.h"
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
    OpenGLDevice::GetInstance()->ApplyPSO(pso_);
    OpenGLDevice::GetInstance()->matrices_ubo()->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_view), sizeof(glm::mat4),
                    glm::value_ptr(camera_.CalcViewMatrix()));
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatricesUniform, t_proj), sizeof(glm::mat4),
                    glm::value_ptr(camera_.CalcPerspectiveProjectionMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    pso_.program_->Use();

    for (Actor *actor : actors_)
    {
        actor->Render(pso_.program_);
    }
}
