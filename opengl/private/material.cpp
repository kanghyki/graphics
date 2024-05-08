#include "material.h"

Material::Material() : textures_{nullptr}
{
}

Material::~Material()
{
}

MaterialPtr Material::Create()
{
    return MaterialPtr(new Material());
}

const char *texture_type_uniform_name[TextureSize::SIZE] = {"material.ambient", "material.diffuse", "material.specular",
                                                            "material.normal", "material.height"};

void Material::SetToProgram(const Program *program) const
{
    int textureCount = 0;

    for (size_t i = 0; i < static_cast<size_t>(TextureSize::SIZE); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + textureCount);
        program->SetUniform(texture_type_uniform_name[i], textureCount);
        textures_[i]->Bind();
        ++textureCount;
    }
    program->SetUniform("material.shininess", uniform_.shininess_);
}
