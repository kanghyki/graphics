#ifndef INCLUDED_RESOURCE_MANAGER_H
#define INCLUDED_RESOURCE_MANAGER_H

#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include <string>

class ResourceManager
{
  public:
    static ResourceManager *GetInstance();
    ~ResourceManager();

    void Init();

    // TODO:
    Texture2dPtr LoadTexture(const std::string &relative_path) const;
    ShaderPtr LoadShader(const std::string &relative_path) const;

  private:
    ResourceManager();
    ResourceManager(const ResourceManager &);
    ResourceManager &operator=(const ResourceManager &);

    static ResourceManager *instance_;
};

#endif
