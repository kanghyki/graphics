#include "event_callback.h"
#include "input_manager.h"
#include "opengl_device.h"
#include "scene_manager.h"
#include "time_manager.h"

int main(void)
{
    OpenGLDevice *device = OpenGLDevice::GetInstance();
    InputManager *input_manager = InputManager::GetInstance();
    TimeManager *time_manager = TimeManager::GetInstance();
    SceneManager *scene_manager = SceneManager::GetInstance();

    device->Init();
    event::glfw_callback_init();
    input_manager->Init();
    scene_manager->Init();
    time_manager->Reset();

    while (!device->IsWindowClose())
    {
        time_manager->Tick();
        input_manager->Update();
        scene_manager->Update();

        /* do something */
        glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene_manager->Render();
        /* do something */

        device->SwapBuffer();
    }
    device->Terminate();

    return 0;
}
