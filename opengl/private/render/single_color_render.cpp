#include "single_color_render.h"

SingleColorRenderPtr SingleColorRender::Create()
{
    auto ptr = SingleColorRenderPtr(new SingleColorRender());
    if (!ptr)
    {
        return nullptr;
    }
    return ptr;
};

SingleColorRender::SingleColorRender()
{
}

SingleColorRender::~SingleColorRender()
{
}

void SingleColorRender::SetUniform(const RenderParams &params)
{
}

void SingleColorRender::Setup()
{
}

void SingleColorRender::AfterFramebufferBind()
{
}

void SingleColorRender::AfterProgramUse()
{
}
