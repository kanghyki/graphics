#include "lighting_render.h"

LightingRenderPtr LightingRender::Create()
{
    auto ptr = LightingRenderPtr(new LightingRender());
    if (!ptr)
    {
        return nullptr;
    }
    return ptr;
};

LightingRender::LightingRender()
{
}

LightingRender::~LightingRender()
{
}

void LightingRender::SetUniform(const RenderParams &params)
{
}

void LightingRender::Setup()
{
}

void LightingRender::AfterFramebufferBind()
{
}

void LightingRender::AfterProgramUse()
{
}
