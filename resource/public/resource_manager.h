#ifndef INCLUDED_RESOURCE_MANAGER_H
#define INCLUDED_RESOURCE_MANAGER_H

#include "directory.h"
#include "opengl_device.h"
#include "texture_resource.h"
#include <string>
#include <vector>

class ResourceManager
{
  public:
    static ResourceManager *GetInstance();
    ~ResourceManager();

    void Init();

    uint32_t LoadTexture(const std::string &relative_path);
    uint32_t LoadModel(const std::string &relative_path);

  private:
    ResourceManager();
    ResourceManager(const ResourceManager &);
    ResourceManager &operator=(const ResourceManager &);

    static ResourceManager *instance_;
    DirectoryPtr root_;
};

#endif
