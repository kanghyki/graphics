#include "camera_manager.h"
#include "camera_component.h"

CameraManager *CameraManager::instance_ = nullptr;

CameraManager *CameraManager::GetInstance()
{
    if (!instance_)
    {
        instance_ = new CameraManager();
    }
    return instance_;
}

CameraManager::CameraManager() : cameras_(0)
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::ClearCamera()
{
    cameras_.clear();
}

void CameraManager::Render()
{
    for (const auto &camera : cameras_)
    {
        camera->Render();
    }
}

void CameraManager::RegisterCamera(CameraComponent *camera)
{
    cameras_.push_back(camera);
}