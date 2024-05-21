#ifndef INCLUDED_MATERIAL_H
#define INCLUDED_MATERIAL_H

#include "program.h"
#include "ptr.h"
#include "texture.h"
#include "uniform_struct.h"
#include <array>

enum class TextureType
{
    AMBIENT = 0,
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT,
    TANGENT
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

  private:
    Material();
    Material(const Material &);
    Material &operator=(const Material &);

    enum
    {
        TextureSize = 6
    };
    const char *texture_type_uniform_name[TextureSize] = {"m_ambient", "m_diffuse", "m_specular",
                                                          "m_normal",  "m_height",  "m_tangent"};
    std::array<Texture2dPtr, TextureSize> textures_;
    float shineness_{30.0f};
};

#endif
