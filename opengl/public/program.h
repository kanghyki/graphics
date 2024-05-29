#ifndef INCLUDED_PROGRAM_H
#define INCLUDED_PROGRAM_H

#include "libmath.h"
#include "ptr.h"
#include "shader.h"
#include "texture.h"
#include <vector>

CLASS_PTR(Program);
class Program
{
  public:
    static ProgramPtr Create(const std::vector<ShaderPtr> &shaders);
    ~Program();

    void Use() const;

    uint32_t GetUniformLocation(const std::string &name) const;
    void SetUniform(const std::string &name, int value) const;
    void SetUniform(const std::string &name, float value) const;
    void SetUniform(const std::string &name, const glm::vec2 &value) const;
    void SetUniform(const std::string &name, const glm::vec3 &value) const;
    void SetUniform(const std::string &name, const glm::vec4 &value) const;
    void SetUniform(const std::string &name, const glm::mat4 &value) const;
    void SetUniform(const std::string &name, const std::vector<glm::mat4> &value) const;

    void ActivateTexture(const std::string &name, const BaseTexture *texture) const;
    void ResetTextureCount() const
    {
        active_texture_count_ = 0;
    }

    const uint32_t id() const
    {
        return id_;
    }

  private:
    Program();
    Program(const Program &);
    Program &operator=(const Program &);

    bool Link(const std::vector<ShaderPtr> &shaders);

    uint32_t id_;

    mutable int active_texture_count_{0};
};

#endif
