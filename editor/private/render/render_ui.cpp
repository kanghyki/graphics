#include "render_ui.h"
#include "renderer.h"

void DrawFramebuffers();
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
    DrawFramebuffers();
    ImGui::End();
}

void DrawFramebuffers()
{
    auto window_size = ImGui::GetWindowSize();
    const auto &main = Renderer::GetInstance()->GetFramebuffer(FramebufferType::MAIN);
    const auto &g_buffer = Renderer::GetInstance()->GetFramebuffer(FramebufferType::G_BUFFER);

    if (ImGui::CollapsingHeader("OPTION", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static bool wire = false;
        if (ImGui::Checkbox("Wire frame", &wire))
        {
            Renderer::GetInstance()->set_wireframe(wire);
        }
    }
    if (ImGui::CollapsingHeader("POST", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Gray scale", &Renderer::GetInstance()->gray_scale_);
        ImGui::DragFloat("Gamma", &Renderer::GetInstance()->gamma_, 0.01f, 0.0f, 3.0f);
    }

    if (ImGui::CollapsingHeader("MAIN", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Image(reinterpret_cast<ImTextureID>(main->color_attachment(0)->id()),
                     ImVec2(window_size.x, window_size.x), ImVec2(0, 1), ImVec2(1, 0));
    }

    if (ImGui::CollapsingHeader("G_BUFFER", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const char *buffer_name[3] = {"Position", "Normal", "Albedo(rgb)/Specular(a)"};
        static int buffer_selected = 0;
        ImGui::Combo("color_attachment", &buffer_selected, buffer_name, 3);
        ImGui::Image(reinterpret_cast<ImTextureID>(g_buffer->color_attachment(buffer_selected)->id()),
                     ImVec2(window_size.x, window_size.x), ImVec2(0, 1), ImVec2(1, 0));
    }
}