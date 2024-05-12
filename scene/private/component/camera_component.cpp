#include "camera_component.h"
#include "camera_manager.h"
#include "level_manager.h"
#include "transform_component.h"

CameraComponent::CameraComponent()
    : Component(ComponentType::CAMERA),

      pitch_(0.0f), yaw_(0.0f), fov_y_(45.0f), aspect_(16.0f / 9.0f),

      near_plane_(0.1f), far_plane_(200.0f), front_(0.0f, 0.0f, -1.0f), up_(0.0f, 1.0f, 0.0f)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Tick()
{
    CameraManager::GetInstance()->RegisterCamera(this);
    // TODO:
}

void CameraComponent::Render()
{
    CalcViewMatrix();
    CalcPerspectiveProjectionMatrix();
    // TODO:
}

glm::mat4 CameraComponent::CalcViewMatrix()
{
    TransformComponent *transform = GetTransformComponent();
    front_ = glm::rotate(glm::mat4(1.0f), glm::radians(yaw_), glm::vec3(0.0f, 1.0f, 0.0f)) *
             glm::rotate(glm::mat4(1.0f), glm::radians(pitch_), glm::vec3(1.0f, 0.0f, 0.0f)) *
             glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    return glm::lookAt(transform->position(), transform->position() + front_, up_);
}

glm::mat4 CameraComponent::CalcPerspectiveProjectionMatrix()
{
    return glm::perspective(glm::radians(fov_y_), aspect_, near_plane_, far_plane_);
}
