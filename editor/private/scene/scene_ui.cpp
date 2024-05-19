#include "scene_ui.h"
#include "actor.h"
#include "camera_component.h"
#include "layer.h"
#include "level.h"
#include "level_manager.h"
#include "light_component.h"
#include "mesh_component.h"
#include "transform_component.h"
#include <spdlog/spdlog.h>

void SceneUI::Update()
{
    level_current = LevelManager::GetInstance()->GetCurrentLevel();
}

void SceneUI::RenderImpl()
{
    Scene();
    ActorDetail();
}

void SceneUI::Scene()
{
    if (ImGui::Begin("Scene"))
    {
        SceneList();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ActorList();
        ImGui::End();
    }
}

void SceneUI::SceneList()
{
    if (ImGui::BeginTable("", 1,
                          ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
    {
        ImGui::TableSetupColumn("List");
        ImGui::TableHeadersRow();
        for (const auto &level : LevelManager::GetInstance()->GetAllLevel())
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            uint32_t flag = ImGuiTreeNodeFlags_Leaf;
            if (level_current == level)
            {
                flag |= ImGuiTreeNodeFlags_Selected;
            }
            if (ImGui::TreeNodeEx(level->name().c_str(), flag))
            {
                if (ImGui::IsItemClicked())
                {
                    LevelManager::GetInstance()->SetCurrentLevel(level->name());
                }
                ImGui::TreePop();
            }
        }
    }
    ImGui::EndTable();
    static char layer_name[512];
    ImGui::InputText("", layer_name, 512);
    if (ImGui::Button("Add layer"))
    {
        level_current->AddLayer(layer_name);
    }
}

void SceneUI::ActorList()
{
    if (ImGui::BeginTable("", 1,
                          ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
    {
        ImGui::TableSetupColumn("Actors");
        ImGui::TableHeadersRow();
        static char buf[512];
        if (ImGui::InputText("", buf, 512))
        {
            SPDLOG_INFO("input");
        }
        for (int i = 0; i < Level::LayerMax; ++i)
        {
            auto layer = level_current->GetLayer(i);
            if (!level_current->GetLayer(i))
            {
                continue;
            }
            if (ImGui::TreeNodeEx(layer->name().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                const auto &actors = layer->actors();
                for (Actor *actor : actors)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    uint32_t flag = ImGuiTreeNodeFlags_Leaf;
                    if (actor_selected == actor)
                    {
                        flag |= ImGuiTreeNodeFlags_Selected;
                    }
                    if (ImGui::TreeNodeEx(actor->name().c_str(), flag))
                    {
                        if (ImGui::IsItemClicked())
                        {
                            actor_selected = actor;
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }
    }
    ImGui::EndTable();
}

void SceneUI::ActorDetail()
{
    if (!actor_selected)
    {
        return;
    }
    if (ImGui::Begin("Detail"))
    {
        ImGui::Text("%s", actor_selected->name().c_str());
        static char buf[512] = {};
        ImGui::InputText("", buf, 512);
        ImGui::SameLine();
        if (ImGui::Button("Change name"))
        {
            actor_selected->set_name(buf);
        }
        ImGui::Text("Pointer : %p", actor_selected);

        CameraDetail();
        TransformDetail();
        LightDetail();
        MeshDetail();
    }
    ImGui::End();
}

void SceneUI::CameraDetail()
{
    std::shared_ptr<CameraComponent> comp = actor_selected->GetCameraComponent();
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (!comp)
        {
            if (ImGui::Button("Create##0"))
            {
                actor_selected->AddCameraComponent();
            }
            return;
        }
        Camera &camera = comp->camera();
        ImGui::DragFloat("FOV Y", &camera.fov_y_, 0.01f);
        ImGui::DragFloat("Aspect", &camera.aspect_, 0.01f);
        ImGui::DragFloat("Near plane", &camera.near_plane_, 0.01f);
        ImGui::DragFloat("Far plane", &camera.far_plane_, 0.01f);
        if (ImGui::Button("Remove##0"))
        {
            actor_selected->RemoveComponent(ComponentType::CAMERA);
        }
    }
}

void SceneUI::TransformDetail()
{

    std::shared_ptr<TransformComponent> comp = actor_selected->GetTransformComponent();
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (!comp)
        {
            if (ImGui::Button("Create##1"))
            {
                actor_selected->AddTransformComponent();
            }
            return;
        }
        Transform &transform = comp->transform();
        ImGui::DragFloat3("Position", glm::value_ptr(transform.position_), 0.01f);
        ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale_), 0.01f);
        ImGui::DragFloat3("Rotation", glm::value_ptr(transform.rotation_), 0.01f);
        if (ImGui::Button("Remove##1"))
        {
            actor_selected->RemoveComponent(ComponentType::TRANSFORM);
        }
    }
}

void SceneUI::LightDetail()
{
    std::shared_ptr<LightComponent> comp = actor_selected->GetLightComponent();
    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (!comp)
        {
            if (ImGui::Button("Create##2"))
            {
                actor_selected->AddLightComponent();
            }
            return;
        }
        Light &light = comp->light();
        ImGui::RadioButton("Directional", (int *)&light.type_, (int)LightType::DIRECTIONAL);
        ImGui::SameLine();
        ImGui::RadioButton("Point", (int *)&light.type_, (int)LightType::POINT);
        ImGui::SameLine();
        ImGui::RadioButton("Spot", (int *)&light.type_, (int)LightType::SPOT);
        if (light.type_ == LightType::DIRECTIONAL || light.type_ == LightType::SPOT)
        {
            ImGui::Text("%10s : x(%.3f), y(%.3f), z(%.3f)", "Direction", light.direction().x, light.direction().y,
                        light.direction().z);
        }
        else if (light.type_ == LightType::SPOT)
        {
            ImGui::DragFloat2("cutoff", glm::value_ptr(light.cutoff_), 0.5f, 0.0f, 180.0f);
        }
        ImGui::Separator();
        ImGui::ColorEdit3("Ambient", glm::value_ptr(light.ambient_));
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.diffuse_));
        ImGui::ColorEdit3("Specular", glm::value_ptr(light.specular_));
        if (ImGui::Button("Remove##2"))
        {
            actor_selected->RemoveComponent(ComponentType::LIGHT);
        }
    }
}

void SceneUI::MeshDetail()
{
    std::shared_ptr<MeshComponent> comp = actor_selected->GetMeshComponent();
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (!comp)
        {
            if (ImGui::Button("Create##3"))
            {
                actor_selected->AddMeshComponent();
            }
            return;
        }
        std::shared_ptr<Mesh> mesh = comp->mesh();
        if (ImGui::Button("Remove##3"))
        {
            actor_selected->RemoveComponent(ComponentType::MESH);
        }
    }
}