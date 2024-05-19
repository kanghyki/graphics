#include "render_ui.h"
#include "opengl_device.h"

void RenderUI::RenderImpl()
{
    static float prev_time = 0;
    static int frames = 0;
    static float fps = 0.0f;
    static int prev_frames = 0;

    frames++;
    float curr_time = (float)glfwGetTime();
    if (curr_time - prev_time >= 1.0)
    {
        prev_frames = frames;
        fps = 1000.0f / frames;
        prev_time = curr_time;
        frames = 0;
    }
    if (ImGui::Begin("Render Info"))
    {
        ImGui::Text("%.3f ms/frame (%dfps)", fps, prev_frames);
    }
    ImGui::End();
}