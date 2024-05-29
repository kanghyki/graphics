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
    for (size_t i = 0; i < static_cast<size_t>(TextureSize); ++i)
    {
        if (textures_[i])
        {
            program->ActivateTexture(texture_type_uniform_name[i], textures_[i].get());
        }
    }
    program->ResetTextureCount();
    Renderer::GetInstance()->GetUBO(UBOType::MATERIAL)->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MaterialUniform), &ToUniform());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
