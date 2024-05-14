#include "camera_manager.h"
#include "event_callback.h"
#include "input_manager.h"
#include "level_manager.h"
#include "level_test.h"
#include "opengl_device.h"
#include "renderer.h"
#include "resource_manager.h"
#include "time_manager.h"
#include "visual_effector.h"

int main(void)
{
    OpenGLDevice *device = OpenGLDevice::GetInstance();
    InputManager *input_manager = InputManager::GetInstance();
    TimeManager *time_manager = TimeManager::GetInstance();
    LevelManager *level_manager = LevelManager::GetInstance();
    CameraManager *camera_manager = CameraManager::GetInstance();
    VisualEffector *visual_effector = VisualEffector::GetInstance();
    ResourceManager *resource_manager = ResourceManager::GetInstance();

    device->Init();
    event::glfw_callback_init();
    Renderer::GetInstance()->Init();

    input_manager->Init();
    level_manager->Init();
    time_manager->Reset();
    resource_manager->Init();

    CreateTestLevel().Create();

    while (!device->IsWindowClose())
    {
        time_manager->Tick();
        input_manager->Update();
        camera_manager->ClearCamera();
        level_manager->Tick();

        camera_manager->Render();
        visual_effector->Render();
    }
    device->Terminate();

    return 0;
}
