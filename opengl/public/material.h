#ifndef INCLUDED_MATERIAL_H
#define INCLUDED_MATERIAL_H

#include "program.h"
#include "ptr.h"
#include "texture.h"

struct MaterialUniform
{
    float shininess_;
};

CLASS_PTR(Material);
class Material
{
  public:
    static MaterialPtr Create();
    ~Material();

    void SetToProgram(const Program *program) const;

  private:
    Material();
    Material(const Material &);
    Material &operator=(const Material &);

    Texture2dPtr diffuse_;
    Texture2dPtr specular_;
    struct MaterialUniform uniform_;
};

#endif
