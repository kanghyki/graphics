#include "camera.h"

Camera::Camera()
    : fov_y_(45.0f), aspect_(16.0f / 9.0f),

      near_plane_(0.1f), far_plane_(200.0f)
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::CalcViewMatrix()
{
    glm::vec3 front = glm::rotate(glm::mat4(1.0f), glm::radians(transform_.rotation_.x), glm::vec3(0.0f, 1.0f, 0.0f)) *
                      glm::rotate(glm::mat4(1.0f), glm::radians(transform_.rotation_.y), glm::vec3(1.0f, 0.0f, 0.0f)) *
                      transform_.direction_;

    return glm::lookAt(transform_.position_, transform_.position_ + front, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::CalcPerspectiveProjectionMatrix()
{
    return glm::perspective(glm::radians(fov_y_), aspect_, near_plane_, far_plane_);
}
