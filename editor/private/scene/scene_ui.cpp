#include "scene_ui.h"
#include "actor.h"
#include "camera_component.h"
#include "layer.h"
#include "level.h"
#include "level_manager.h"
#include "light_component.h"
#include "model_component.h"
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
        static char scene_name[512];
        ImGui::InputText("##scene_actor", scene_name, 512);
        ImGui::SameLine();
        if (ImGui::Button("Add scene"))
        {
            LevelManager::GetInstance()->AddLevel(scene_name);
        }
        SceneList();

        ImGui::Separator();
        static char layer_name[512];
        ImGui::InputText("##add_layer", layer_name, 512);
        ImGui::SameLine();
        if (ImGui::Button("Add Layer"))
        {
            level_current->AddLayer(layer_name);
        }

        ActorList();
    }
    ImGui::End();
}

void SceneUI::SceneList()
{
    if (ImGui::BeginTable("", 1,
                          ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
    {
        ImGui::TableSetupColumn("Scenes");
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
}

void SceneUI::ActorList()
{
    if (ImGui::BeginTable("", 1,
                          ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody))
    {
        ImGui::TableSetupColumn("Actors");
        ImGui::TableHeadersRow();
        for (int i = 0; i < Level::LayerMax; ++i)
        {
            auto layer = level_current->GetLayer(i);
            if (!level_current->GetLayer(i))
            {
                continue;
            }
            if (ImGui::TreeNodeEx(layer->name().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                {
                    static char actor_name[512];
                    std::string add_actor_id("##" + layer->name());
                    ImGui::InputText(add_actor_id.c_str(), actor_name, 512);
                    ImGui::SameLine();
                    if (ImGui::Button("Add actor"))
                    {
                        layer_selected->AddActor(new Actor(actor_name));
                    }
                }
                if (ImGui::IsItemClicked())
                {
                    layer_selected = layer;
                }
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
        ModelDetail();
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

void SceneUI::ModelDetail()
{
    std::shared_ptr<ModelComponent> comp = actor_selected->GetModelComponent();
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (!comp)
        {
            if (ImGui::Button("Create##3"))
            {
                actor_selected->AddModelComponent();
            }
            return;
        }
        auto model = comp->model();
        if (model)
        {
            for (uint32_t i = 0; i < model->meshes_count(); ++i)
            {
                auto mesh = model->mesh(i);
                if (!mesh->material())
                {
                    if (ImGui::Button("Create material"))
                    {
                        mesh->set_material(Material::Create());
                    }
                    ImGui::SameLine();
                    ImGui::Text(" : (None)");
                    return;
                }
                ImGui::Text("Ambient");
                MeshTextureDetail(mesh, TextureType::AMBIENT);
                ImGui::Text("Diffuse");
                MeshTextureDetail(mesh, TextureType::DIFFUSE);
                ImGui::Text("Specular");
                MeshTextureDetail(mesh, TextureType::SPECULAR);
                ImGui::Text("Normal");
                MeshTextureDetail(mesh, TextureType::NORMAL);
                ImGui::Text("Height");
                MeshTextureDetail(mesh, TextureType::HEIGHT);
                ImGui::Text("Tangent");
                MeshTextureDetail(mesh, TextureType::TANGENT);
            }
        }
        if (model_selected)
        {
            if (ImGui::Button("Add model"))
            {
                actor_selected->GetModelComponent()->set_model(model_selected);
            }
        }
        if (ImGui::Button("Remove##3"))
        {
            actor_selected->RemoveComponent(ComponentType::MODEL);
        }
    }
}

void SceneUI::MeshTextureDetail(MeshPtr mesh, TextureType type)
{
    auto tex = mesh->material()->texture(type);
    if (!tex)
    {
        ImGui::SameLine();
        ImGui::Text(" : (None)");

        if (texture2d_selected)
        {
            ImGui::SameLine();
            std::string label("Set texture##" + std::to_string((int)type));
            if (ImGui::Button(label.c_str()))
            {
                mesh->material()->set_texture(type, texture2d_selected);
            }
        }
        return;
    }
    ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(tex->id())), ImVec2(100, 100));
    std::string label("Remove texture##" + std::to_string((int)type));
    if (ImGui::Button(label.c_str()))
    {
        mesh->material()->set_texture(type, nullptr);
    }

    if (texture2d_selected)
    {
        std::string label("Change texture##" + std::to_string((int)type));
        if (ImGui::Button(label.c_str()))
        {
            mesh->material()->set_texture(type, texture2d_selected);
        }
    }
}