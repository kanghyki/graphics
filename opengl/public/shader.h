#ifndef INCLUDED_SHADER_H
#define INCLUDED_SHADER_H

#include "libopengl.h"
#include "ptr.h"
#include <string>

CLASS_PTR(Shader);
class Shader
{
  public:
    static ShaderPtr CreateFromFile(const std::string &filename, GLenum shader_type);
    ~Shader();

    const uint32_t id() const
    {
        return id_;
    }

  private:
    Shader();
    Shader(const Shader &);
    Shader &operator=(const Shader &);

    bool LoadFile(const std::string &filename, GLenum shader_type);
    void Preprocess(std::string &code, const std::string &filename) const;

    uint32_t id_;
};

#endif
