#ifndef INCLUDED_RESOURCE_MANAGER_H
#define INCLUDED_RESOURCE_MANAGER_H

#include "model.h"
#include "texture.h"
#include <string>
#include <utility>
#include <vector>

class ResourceManager
{
  public:
    static ResourceManager *GetInstance();
    ~ResourceManager();

    void Init();
    bool LoadResource(const std::string &path);

    std::vector<std::pair<std::string, ModelPtr>> models()
    {
        return models_;
    }
    std::vector<Texture2dPtr> texture2ds()
    {
        return texture2ds_;
    }

  private:
    ResourceManager();
    ResourceManager(const ResourceManager &);
    ResourceManager &operator=(const ResourceManager &);

    bool LoadTexture(const std::string &path);
    bool LoadModel(const std::string &path);

    static ResourceManager *instance_;
    std::vector<std::pair<std::string, ModelPtr>> models_;
    std::vector<Texture2dPtr> texture2ds_;
};

#endif
