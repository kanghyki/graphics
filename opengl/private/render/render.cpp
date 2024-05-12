#include "render.h"

Render::Render() : framebuffer_(nullptr), program_(nullptr), uniform_buffer_(nullptr)
{
}

Render::~Render()
{
}

void Render::Use(const RenderParams &params)
{
    SetUniform(params);
    framebuffer_->Bind();
    AfterFramebufferBind();
    program_->Use();
    AfterProgramUse();
}
