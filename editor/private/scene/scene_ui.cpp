#include "scene_ui.h"
#include "actor.h"
#include "camera_component.h"
#include "layer.h"
#include "level.h"
#include "level_manager.h"
#include "light_component.h"
#include "model_component.h"
#include "skybox_component.h"
#include "terrain_component.h"
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
        SkyboxDetail();
        TerrainDetail();
    }
    ImGui::End();
}

void SceneUI::CameraDetail()
{
    CameraComponent *cc = actor_selected->GetCameraComponent();
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (!cc)
        {
            if (ImGui::Button("Create##0"))
            {
                actor_selected->AddCameraComponent();
            }
            return;
        }
        ImGui::DragFloat("FOV Y", cc, &CameraComponent::fov_y, &CameraComponent::set_fov_y, 0.01f);
        ImGui::DragFloat("Aspect", cc, &CameraComponent::aspect, &CameraComponent::set_aspect, 0.01f);
        ImGui::DragFloat("Near plane", cc, &CameraComponent::near_plane, &CameraComponent::set_near_plane, 0.01f);
        ImGui::DragFloat("Far plane", cc, &CameraComponent::far_plane, &CameraComponent::set_far_plane, 0.01f);
        if (ImGui::Button("Remove##0"))
        {
            actor_selected->RemoveComponent(ComponentType::CAMERA);
        }
    }
}

void SceneUI::TransformDetail()
{

    TransformComponent *tc = actor_selected->GetTransformComponent();
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (!tc)
        {
            if (ImGui::Button("Create##1"))
            {
                actor_selected->AddTransformComponent();
            }
            return;
        }
        ImGui::DragFloat3("Position", tc, &TransformComponent::position, &TransformComponent::set_position, 0.02f);
        ImGui::DragFloat3("Scale", tc, &TransformComponent::scale, &TransformComponent::set_scale, 0.02f);
        ImGui::DragFloat3("Rotation", tc, &TransformComponent::rotation, &TransformComponent::set_rotation, 0.02f);
        if (ImGui::Button("Remove##1"))
        {
            actor_selected->RemoveComponent(ComponentType::TRANSFORM);
        }
    }
}

void SceneUI::LightDetail()
{
    LightComponent *comp = actor_selected->GetLightComponent();
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
        ImGui::Checkbox("Shadow", comp, &LightComponent::use_shadow, &LightComponent::set_use_shadow);
        ImGui::RadioButton<LightType>("Directional", comp, &LightComponent::type, &LightComponent::set_type,
                                      (int)LightType::DIRECTIONAL);
        ImGui::SameLine();
        ImGui::RadioButton<LightType>("Point", comp, &LightComponent::type, &LightComponent::set_type,
                                      (int)LightType::POINT);
        ImGui::SameLine();
        ImGui::RadioButton<LightType>("Spot", comp, &LightComponent::type, &LightComponent::set_type,
                                      (int)LightType::SPOT);
        ImGui::Text("%10s : x(%.3f), y(%.3f), z(%.3f)", "Direction", comp->direction().x, comp->direction().y,
                    comp->direction().z);
        ImGui::ColorEdit3("color", comp, &LightComponent::color, &LightComponent::set_color);
        ImGui::SliderFloat("strength", comp, &LightComponent::strength, &LightComponent::set_strength, 0.0f, 100.0f);
        ImGui::SliderFloat("falloff_start", comp, &LightComponent::falloff_start, &LightComponent::set_falloff_start,
                           0.0f, 100.0f);
        ImGui::SliderFloat("falloff_end", comp, &LightComponent::falloff_end, &LightComponent::set_falloff_end, 0.0f,
                           100.0f);
        ImGui::SliderFloat("spot_power", comp, &LightComponent::spot_power, &LightComponent::set_spot_power, 0.0f,
                           100.0f);
        ImGui::Separator();
        if (comp->depth_map())
        {
            ImGui::Image(reinterpret_cast<ImTextureID>(comp->depth_map()->texture()->id()), ImVec2(100, 100));
        }
        if (ImGui::Button("Remove##2"))
        {
            actor_selected->RemoveComponent(ComponentType::LIGHT);
        }
    }
}

void SceneUI::ModelDetail()
{
    ModelComponent *comp = actor_selected->GetModelComponent();
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

                if (ImGui::TreeNodeEx(std::to_string(i).c_str()))
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
                    }
                    else
                    {
                        ImGui::DragFloat("Shininess", &mesh->material()->shineness_, 0.01f, 0.0f, 100.0f);
                        ImGui::ColorEdit3("Albedo", glm::value_ptr(mesh->material()->albedo_color_));
                        ImGui::DragFloat("Specular alpha", &mesh->material()->specular_alpha_, 0.01f, 0.0f, 1.0f);
                        ImGui::DragFloat("Height scale", &mesh->material()->height_scale_, 0.01f, 0.0f, 1.0f);
                        ImGui::Text("ALBEDO");
                        MeshTextureDetail(mesh, TextureType::ALBEDO);
                        ImGui::Text("Specular");
                        MeshTextureDetail(mesh, TextureType::SPECULAR);
                        ImGui::Text("AO");
                        MeshTextureDetail(mesh, TextureType::AO);
                        ImGui::Text("Normal");
                        MeshTextureDetail(mesh, TextureType::NORMAL);
                        ImGui::Text("Height");
                        MeshTextureDetail(mesh, TextureType::HEIGHT);
                        ImGui::Text("Roughness");
                        MeshTextureDetail(mesh, TextureType::ROUGHNESS);
                        ImGui::Text("Metaillic");
                        MeshTextureDetail(mesh, TextureType::METAILLIC);
                        ImGui::Text("Emissive");
                        MeshTextureDetail(mesh, TextureType::EMISSIVE);
                    }

                    ImGui::TreePop();
                }
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
    ImGui::Image(reinterpret_cast<ImTextureID>(tex->id()), ImVec2(100, 100));
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

void SceneUI::SkyboxDetail()
{
    SkyboxComponent *skybox_component = actor_selected->GetSkyboxComponent();

    if (ImGui::CollapsingHeader("Skybox", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (skybox_component)
        {
            ImGui::Checkbox("Activate", skybox_component, &SkyboxComponent::activate, &SkyboxComponent::set_activate);
            if (ImGui::Button("Remove##skyboxremove"))
            {
                actor_selected->RemoveComponent(ComponentType::SKYBOX);
            }
        }
        else
        {
            if (ImGui::Button("Create##skyboxcreate"))
            {
                actor_selected->AddSkyboxComponent();
            }
        }
    }
}

void SceneUI::TerrainDetail()
{
    TerrainComponent *terrain_component = actor_selected->GetTerrainComponent();
    if (ImGui::CollapsingHeader("Terrain", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (terrain_component)
        {
            ImGui::DragFloat("Height scale", terrain_component, &TerrainComponent::height_scale,
                             &TerrainComponent::set_height_scale, 0.01f);
            if (ImGui::Button("Remove##terrainremove"))
            {
                actor_selected->RemoveComponent(ComponentType::TERRAIN);
            }
        }
        else
        {
            if (ImGui::Button("Create##terraincreate"))
            {
                actor_selected->AddTerrainComponent();
            }
        }
    }
}