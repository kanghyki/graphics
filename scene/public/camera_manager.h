#ifndef INCLUDED_CAMERA_MANAGER_H
#define INCLUDED_CAMERA_MANAGER_H

#include <vector>

class CameraComponent;
class CameraManager
{
  public:
    static CameraManager *GetInstance();
    ~CameraManager();

    void ClearCamera();
    void Render();
    void SetMainCamera(CameraComponent *camera);

  private:
    CameraManager();
    CameraManager(const CameraManager &);
    CameraManager &operator=(const CameraManager &);

    static CameraManager *instance_;

    CameraComponent *main_camera_;
};

#endif
