#include "resource_manager.h"

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
}
