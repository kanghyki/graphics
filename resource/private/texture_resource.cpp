#include "texture_resource.h"

TextureResourcePtr TextureResource::Create()
{
    auto resource = TextureResourcePtr(new TextureResource());

    return resource;
}

TextureResource::TextureResource()
{
}

TextureResource::~TextureResource()
{
}

void TextureResource::LoadImpl(const std::string &full_path)
{
}