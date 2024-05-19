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
    root_ = Directory::Create(RESOURCE_DIR_PATH);
    root_->Load();
}

uint32_t ResourceManager::LoadTexture(const std::string &relative_path)
{
    return 1;
}

uint32_t ResourceManager::LoadModel(const std::string &relative_path)
{
    return 1;
}
