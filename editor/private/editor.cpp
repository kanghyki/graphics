#include "editor.h"
#include "opengl_device.h"
#include <string>

Editor *Editor::instance_ = nullptr;

Editor *Editor::GetInstance()
{
    if (!instance_)
    {
        instance_ = new Editor();
    }

    return instance_;
}

Editor::Editor()
{
}

Editor::~Editor()
{
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imgui_context_);
}

void Editor::Init(void *window)
{
    imgui_context_ = ImGui::CreateContext();
    ImGui::SetCurrentContext(imgui_context_);
    ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow *>(window), false);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();
}

void Editor::NewFrame()
{
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Editor::Render()
{
    if (ImGui::Begin("Test"))
    {
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}