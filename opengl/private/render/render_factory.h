#ifndef INCLUDED_RENDER_FACTORY_H
#define INCLUDED_RENDER_FACTORY_H

#include "render.h"

enum class RenderType
{
    LIGHTING,
    SINGLE_COLOR,
    EOL
};

class RenderFactory
{
  public:
    static RenderPtr CreateRender(RenderType type);
    RenderFactory();
    ~RenderFactory();

  private:
    RenderFactory(const RenderFactory &);
    RenderFactory &operator=(const RenderFactory &);
};

#endif
