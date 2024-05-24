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

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::ClearCamera()
{
    main_camera_ = nullptr;
}

void CameraManager::Render()
{
    main_camera_->Render();
}

void CameraManager::SetMainCamera(CameraComponent *camera)
{
    main_camera_ = camera;
}