#ifndef INCLUDED_COMPONENT_FACTORY_H
#define INCLUDED_COMPONENT_FACTORY_H

#include "component_type.h"
#include <memory>

class Component;
class ComponentFactory
{
  public:
    ComponentFactory();
    ~ComponentFactory();

    std::shared_ptr<Component> Generate(ComponentType type) const;
};

#endif