#include "editor.h"
#include "main_menu.h"
#include "render_ui.h"
#include "resource_ui.h"
#include "scene_ui.h"

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
    ui_ = std::unique_ptr<EditorUI>(new EditorUI());
    ui_->AddUI(std::unique_ptr<EditorUI>(new MainMenu()));
    ui_->AddUI(std::unique_ptr<EditorUI>(new SceneUI()));
    ui_->AddUI(std::unique_ptr<EditorUI>(new ResourceUI()));
    ui_->AddUI(std::unique_ptr<EditorUI>(new RenderUI()));
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
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Editor::Render()
{
    ui_->Render();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}