#include "material.h"
#include "renderer.h"

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
    int textureCount = 0;
    for (size_t i = 0; i < static_cast<size_t>(TextureSize); ++i)
    {
        if (textures_[i])
        {
            glActiveTexture(GL_TEXTURE0 + textureCount);
            textures_[i]->Bind();
            program->SetUniform(texture_type_uniform_name[i], textureCount);
            ++textureCount;
        }
    }
    Renderer::GetInstance()->GetUBO(UBOType::MATERIAL)->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MaterialUniform), &ToUniform());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
