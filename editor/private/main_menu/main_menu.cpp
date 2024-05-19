#include "main_menu.h"
#include "time_manager.h"

void MainMenu::RenderImpl()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Timer"))
        {
            if (ImGui::MenuItem("Start"))
            {
                TimeManager::GetInstance()->Start();
            }
            if (ImGui::MenuItem("Stop"))
            {
                TimeManager::GetInstance()->Stop();
            }
            if (ImGui::MenuItem("Reset"))
            {
                TimeManager::GetInstance()->Reset();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}