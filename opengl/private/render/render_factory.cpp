#include "lighting_render.h"
#include "render_factory.h"
#include "single_color_render.h"

RenderPtr RenderFactory::CreateRender(RenderType type)
{
    RenderPtr ret = nullptr;

    if (type == RenderType::LIGHTING)
    {
        ret = LightingRender::Create();
    }
    else if (type == RenderType::SINGLE_COLOR)
    {
        ret = SingleColorRender::Create();
    }

    return ret;
}

RenderFactory::RenderFactory()
{
}

RenderFactory::~RenderFactory()
{
}
