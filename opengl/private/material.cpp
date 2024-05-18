#include "material.h"
#include "opengl_device.h"

Material::Material()
{
    textures_.fill(nullptr);
}

Material::~Material()
{
}

MaterialPtr Material::Create()
{
    return MaterialPtr(new Material());
}

void Material::Setup(const Program *program) const
{
    uniform_.Sub();

    int textureCount = 0;

    for (size_t i = 0; i < static_cast<size_t>(TextureSize); ++i)
    {
        if (textures_[i])
        {
            glActiveTexture(GL_TEXTURE0 + textureCount);
            program->SetUniform(texture_type_uniform_name[i], textureCount);
            textures_[i]->Bind();
            ++textureCount;
        }
    }
}
