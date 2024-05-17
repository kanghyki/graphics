#ifndef INCLUDED_MATERIAL_H
#define INCLUDED_MATERIAL_H

#include "program.h"
#include "ptr.h"
#include "texture.h"
#include "uniform_struct.h"

enum class TextureType
{
    AMBIENT = 0,
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT
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
        uniform_.m_float_0 = shineness;
    }

    Texture2dPtr texture(TextureType type)
    {
        return textures_[static_cast<int>(type)];
    }
    void set_texture(TextureType type, Texture2dPtr texture)
    {
        textures_[static_cast<int>(type)] = texture;
    }

    MaterialUniform &uniform()
    {
        return uniform_;
    }
    const MaterialUniform &uniform() const
    {
        return uniform_;
    }

  private:
    Material();
    Material(const Material &);
    Material &operator=(const Material &);

    enum
    {
        TextureSize = 5
    };
    const char *texture_type_uniform_name[TextureSize] = {"m_ambient", "m_diffuse", "m_specular", "m_normal",
                                                          "m_height"};
    Texture2dPtr textures_[TextureSize];
    MaterialUniform uniform_;
};

#endif
