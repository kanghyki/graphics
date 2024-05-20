#include "editor.h"
#include "main_menu.h"
#include "render_ui.h"
#include "resource_ui.h"
#include "scene_ui.h"
#include <memory>

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
    ui_ = std::shared_ptr<EditorUI>(new EditorUI());
    auto main_menu = std::shared_ptr<MainMenu>(new MainMenu());
    auto resource_ui = std::shared_ptr<ResourceUI>(new ResourceUI());
    auto scene_ui = std::shared_ptr<SceneUI>(new SceneUI());
    resource_ui->Register(scene_ui.get());
    auto render_ui = std::shared_ptr<RenderUI>(new RenderUI());
    ui_->AddUI(std::reinterpret_pointer_cast<EditorUI>(main_menu));
    ui_->AddUI(std::reinterpret_pointer_cast<EditorUI>(scene_ui));
    ui_->AddUI(std::reinterpret_pointer_cast<EditorUI>(resource_ui));
    ui_->AddUI(std::reinterpret_pointer_cast<EditorUI>(render_ui));
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