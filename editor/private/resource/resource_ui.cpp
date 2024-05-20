#include "resource_ui.h"
#include "resource_manager.h"
#include <spdlog/spdlog.h>

void ResourceUI::RenderImpl()
{
    if (ImGui::Begin("Resource"))
    {
        auto TexturePreview = [this](const Texture2dPtr tex) {
            ImGui::SameLine();
            if (!tex)
            {
                ImGui::Text(" : (None)");
                return;
            }
            if (texture2d_selected == tex)
            {
                ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(tex->id())), ImVec2(130, 130));
                if (ImGui::IsItemClicked())
                {
                    texture2d_selected = nullptr;
                    NotifyTexture2d(texture2d_selected);
                }
            }
            else
            {
                ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(tex->id())), ImVec2(100, 100));
                if (ImGui::IsItemClicked())
                {
                    texture2d_selected = tex;
                    NotifyTexture2d(texture2d_selected);
                }
            }
        };

        for (const auto &texture2d : ResourceManager::GetInstance()->texture2ds())
        {
            TexturePreview(texture2d);
        }

        auto models = ResourceManager::GetInstance()->models();
        int i = 0;
        for (int i = 0; i < models.size(); ++i)
        {
            static auto selectable = -1;
            std::string itemid("##" + std::to_string(i));
            if (ImGui::Selectable(itemid.c_str(), i == selectable))
            {
                if (i == selectable)
                {
                    model_selected = nullptr;
                    selectable = -1;
                }
                else
                {
                    model_selected = models[i];
                    selectable = i;
                }
                NotifyModel(model_selected);
            }
            ImGui::SameLine();
            ImGui::Text("Model %d", i);
        }
    }
    ImGui::End();
}