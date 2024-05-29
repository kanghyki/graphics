#include "program.h"
#include <spdlog/spdlog.h>

Program::Program() : id_(0)
{
}

Program::~Program()
{
    if (id_)
    {
        glDeleteProgram(id_);
    }
}

ProgramPtr Program::Create(const std::vector<ShaderPtr> &shaders)
{
    auto program = ProgramPtr(new Program());
    if (!program->Link(shaders))
    {
        return nullptr;
    }

    return program;
}

void Program::Use() const
{
    active_texture_count_ = 0;
    glUseProgram(id_);
}

bool Program::Link(const std::vector<ShaderPtr> &shaders)
{
    id_ = glCreateProgram();
    for (auto &shader : shaders)
    {
        glAttachShader(id_, shader->id());
    }
    glLinkProgram(id_);

    int success = 0;
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(id_, 512, nullptr, infoLog);
        SPDLOG_ERROR("ERROR::PROGRAM::LINKING_FAILED");
        SPDLOG_ERROR("{}", infoLog);
    }

    return success;
}

uint32_t Program::GetUniformLocation(const std::string &name) const
{
    return glGetUniformLocation(id_, name.c_str());
}

void Program::SetUniform(const std::string &name, int value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniform1i(loc, value);
}

void Program::SetUniform(const std::string &name, float value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniform1f(loc, value);
}

void Program::SetUniform(const std::string &name, const glm::vec2 &value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniform2fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const glm::vec3 &value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const glm::vec4 &value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const glm::mat4 &value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const std::vector<glm::mat4> &value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniformMatrix4fv(loc, value.size(), GL_FALSE, glm::value_ptr(*(value.data())));
}

void Program::ActivateTexture(const std::string &name, const BaseTexture *texture) const
{
    glActiveTexture(GL_TEXTURE0 + active_texture_count_);
    texture->Bind();
    SetUniform(name, active_texture_count_);
    ++active_texture_count_;
}
