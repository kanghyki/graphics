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
    void RegisterCamera(CameraComponent *camera);

  private:
    CameraManager();
    CameraManager(const CameraManager &);
    CameraManager &operator=(const CameraManager &);

    static CameraManager *instance_;

    std::vector<CameraComponent *> cameras_;
};

#endif
