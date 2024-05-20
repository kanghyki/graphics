#include "resource_manager.h"
#include <filesystem>
#include <spdlog/spdlog.h>

ResourceManager *ResourceManager::instance_ = nullptr;

ResourceManager *ResourceManager::GetInstance()
{
    if (!instance_)
    {
        instance_ = new ResourceManager();
    }
    return instance_;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Init()
{
    std::string path(RESOURCE_DIR_PATH);
    /* Texture2d */
    for (const auto &entry : std::filesystem::directory_iterator(path + "/texture2d"))
    {
        if (entry.is_regular_file())
        {
            auto texture2d = Texture2d::Load(entry.path().string());
            if (!texture2d)
            {
                SPDLOG_WARN("Loading failed 2d Texture : {}", entry.path().filename().string());
                continue;
            }
            SPDLOG_INFO("Loaded 2d texture : {}", entry.path().filename().string());
            texture2ds_.push_back(texture2d);
        }
    }
    /* Model */

    models_.push_back(Model::Create({Mesh::CreateBox()}));
    models_.push_back(Model::Create({Mesh::CreateSphere(30, 30)}));
    models_.push_back(Model::Create({Mesh::CreatePlane()}));
    for (const auto &entry : std::filesystem::directory_iterator(path + "/model"))
    {
        if (entry.is_regular_file())
        {
            auto model = Model::Load(entry.path().string());
            if (!model)
            {
                SPDLOG_WARN("Loading failed Model : {}", entry.path().filename().string());
                continue;
            }
            SPDLOG_INFO("Loaded Model : {}", entry.path().filename().string());
            models_.push_back(model);
        }
    }
}