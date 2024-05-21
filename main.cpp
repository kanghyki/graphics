#include "camera_manager.h"
#include "editor.h"
#include "event_callback.h"
#include "input_manager.h"
#include "level_manager.h"
#include "level_test.h"
#include "light_manager.h"
#include "opengl_device.h"
#include "renderer.h"
#include "resource_manager.h"
#include "time_manager.h"

int main(void)
{
    OpenGLDevice *device = OpenGLDevice::GetInstance();
    InputManager *input_manager = InputManager::GetInstance();
    TimeManager *time_manager = TimeManager::GetInstance();
    LevelManager *level_manager = LevelManager::GetInstance();
    CameraManager *camera_manager = CameraManager::GetInstance();
    ResourceManager *resource_manager = ResourceManager::GetInstance();
    LightManager *light_manager = LightManager::GetInstance();
    Renderer *renderer = Renderer::GetInstance();

    // Must follow this order
    device->Init();
#ifdef EDITOR
    Editor *editor = Editor::GetInstance();
    editor->Init(device->glfw_window());
#endif
    event::glfw_callback_init();
    renderer->Init();
    input_manager->Init();
    level_manager->Init();
    resource_manager->Init();
    light_manager->Init();
    time_manager->Reset();

    CreateTestLevel().Create();

    while (!device->IsWindowClose())
    {
        time_manager->Tick();
        input_manager->Update();
        camera_manager->ClearCamera();
        level_manager->Tick();
        light_manager->Tick();

        renderer->ClearFramebuffer();
        camera_manager->Render();
        renderer->Render();
        renderer->PostProcessing();
#ifdef EDITOR
        editor->NewFrame();
        editor->Render();
#endif

        device->SwapBuffer();
    }
    device->Terminate();

    return 0;
}
