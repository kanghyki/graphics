#ifndef INCLUDED_MATERIAL_H
#define INCLUDED_MATERIAL_H

#include "program.h"
#include "ptr.h"
#include "texture.h"

struct MaterialUniform
{
    float shininess_{30.0f};
};

enum TextureSize
{
    SIZE = 5
};

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

    void SetToProgram(const Program *program) const;
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

    Texture2dPtr textures_[static_cast<int>(TextureSize::SIZE)];
    struct MaterialUniform uniform_;
};

#endif
