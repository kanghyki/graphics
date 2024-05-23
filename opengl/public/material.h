#ifndef INCLUDED_MATERIAL_H
#define INCLUDED_MATERIAL_H

#include "program.h"
#include "ptr.h"
#include "texture.h"
#include "uniform_struct.h"
#include <array>

/*
 * "include/defualt.incl"
 * must keep in sync with glsl
 */
enum class TextureType
{
    ALBEDO = 0,
    SPECULAR,
    AO,
    NORMAL,
    ROUGHNESS,
    METAILLIC,
    EMISSIVE
};

CLASS_PTR(Material);
class Material
{
  public:
    static MaterialPtr Create();
    ~Material();

    void Setup(const Program *program) const;

    void set_shineness(float shineness)
    {
        shineness_ = shineness;
    }

    Texture2dPtr texture(TextureType type)
    {
        return textures_[static_cast<int>(type)];
    }
    void set_texture(TextureType type, Texture2dPtr texture)
    {
        textures_[static_cast<int>(type)] = texture;
    }

    float shineness_{30.0f};
    glm::vec3 albedo_color_{1.0f, 0.0f, 0.0f};
    float specular_alpha_{1.0f};

  private:
    Material();
    Material(const Material &);
    Material &operator=(const Material &);

    /*
     * "include/defualt.incl"
     * must keep in sync with glsl
     */
    enum
    {
        TextureSize = 7
    };
    const char *texture_type_uniform_name[TextureSize] = {
        // clang-format off
            "m_albedo",
            "m_specular",
            "m_ao",
            "m_normal",
            "m_roughness",
            "m_metallic",
            "m_emissive"
        // clang-format on
    };
    std::array<Texture2dPtr, TextureSize> textures_;
    MaterialUniform ToUniform() const
    {
        MaterialUniform uniform;
        memset(&uniform, 0, sizeof(MaterialUniform));

        uniform.m_shineness = shineness_;
        uniform.albedo_color = albedo_color_;
        uniform.specular_alpha = specular_alpha_;
        if (textures_[0])
        {
            uniform.use_albedo_map = true;
        }
        if (textures_[1])
        {
            uniform.use_specular_map = true;
        }
        if (textures_[2])
        {
            uniform.use_ao_map = true;
        }
        if (textures_[3])
        {
            uniform.use_normal_map = true;
        }
        if (textures_[4])
        {
            uniform.use_roughness_map = true;
        }
        if (textures_[5])
        {
            uniform.use_metallic_map = true;
        }
        if (textures_[6])
        {
            uniform.use_emissive_map = true;
        }
        return uniform;
    }
};

#endif
