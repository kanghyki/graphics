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
    /* Texture */
    for (const auto &entry : std::filesystem::directory_iterator(path + "/texture2d"))
    {
        if (entry.is_regular_file())
        {
            LoadTexture(entry.path().string());
        }
    }

    /* Model */
    models_.push_back({"box", Model::Create({Mesh::CreateBox()})});
    models_.push_back({"sphere", Model::Create({Mesh::CreateSphere(30, 30)})});
    models_.push_back({"plane", Model::Create({Mesh::CreatePlane()})});
    for (const auto &entry : std::filesystem::directory_iterator(path + "model"))
    {
        if (entry.is_regular_file())
        {
            if (entry.path().string().find(".obj") == std::string::npos)
            {
                continue;
            }
            LoadModel(entry.path().string());
        }
    }
}

bool ResourceManager::LoadResource(const std::string &path)
{
    bool ret = false;

    size_t pos = path.find_last_of(".");
    if (pos == std::string::npos)
    {
        SPDLOG_WARN("Extension not found.");
        return false;
    }
    std::string extension = path.substr(pos);
    if (extension == ".png" || extension == ".jpg")
    {
        ret = LoadTexture(path);
    }
    else if (extension == ".obj")
    {
        ret = LoadModel(path);
    }

    return ret;
}

bool ResourceManager::LoadTexture(const std::string &path)
{
    SPDLOG_INFO("Load.. Texture {}", path);
    auto texture2d = Texture::Create(Image::Load(path, false).get());
    if (!texture2d)
    {
        SPDLOG_WARN("Failed");
        return false;
    }
    texture2ds_.push_back(texture2d);
    SPDLOG_INFO("Success");

    return true;
}

bool ResourceManager::LoadModel(const std::string &path)
{
    SPDLOG_INFO("Load.. Model {}", path);
    auto model = Model::Load(path);
    if (!model)
    {
        SPDLOG_WARN("Failed");
        return false;
    }
    models_.push_back({path, model});
    SPDLOG_INFO("Success");

    return true;
}