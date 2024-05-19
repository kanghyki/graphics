#ifndef INCLUDED_TEXTURE_RESOURCE_H
#define INCLUDED_TEXTURE_RESOURCE_H

#include "opengl_device.h"
#include "ptr.h"
#include "resource.h"

CLASS_PTR(TextureResource);
class TextureResource : public Resource
{
  public:
    static TextureResourcePtr Create();
    ~TextureResource();

    Texture2dPtr texture()
    {
        return texture_;
    }

  private:
    TextureResource();
    TextureResource(const TextureResource &rhs);
    TextureResource &operator=(const TextureResource &rhs);

    void LoadImpl(const std::string &full_path) override;

    Texture2dPtr texture_;
};

#endif