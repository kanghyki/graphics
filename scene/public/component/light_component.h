#ifndef INCLUDED_LIGHT_COMPONENT_H
#define INLCUDED_LIGHT_COMPONENT_H

#include "component.h"

class LightComponent : public Component
{
  public:
    LightComponent();
    ~LightComponent();

  private:
    LightComponent(const LightComponent &);
    LightComponent &operator=(const LightComponent &);
};

#endif