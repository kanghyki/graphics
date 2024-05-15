#ifndef INCLUDED_MATERIAL_H
#define INCLUDED_MATERIAL_H

#include "program.h"
#include "ptr.h"
#include "texture.h"

struct MaterialUniform
{
    float shininess_{30.0f};
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

    enum
    {
        TextureSize = 5
    };
    const char *texture_type_uniform_name[TextureSize] = {"material.ambient", "material.diffuse", "material.specular",
                                                          "material.normal", "material.height"};
    Texture2dPtr textures_[TextureSize];
    struct MaterialUniform uniform_;
};

#endif
