#ifndef INCLUDED_COMPONENT_H
#define INCLUDED_COMPONENT_H

#include "component_type.h"
#include <memory>

class TransformComponent;
class CameraComponent;
class LightComponent;
class ModelComponent;
class SkyboxComponent;
#define OWNER_COMPONENT_GETTER_DECL(className) className *Get##className() const;
#define OWNER_COMPONENT_GETTER_IMPL(className)                                                                         \
    className *Component::Get##className() const                                                                       \
    {                                                                                                                  \
        return owner_->Get##className();                                                                               \
    }

class Actor;
class Component
{
    friend class Actor;

  public:
    Component(ComponentType type);
    virtual ~Component();

    virtual void Tick();
    virtual void FinalTick();

    ComponentType type()
    {
        return type_;
    }

    OWNER_COMPONENT_GETTER_DECL(TransformComponent)
    OWNER_COMPONENT_GETTER_DECL(CameraComponent)
    OWNER_COMPONENT_GETTER_DECL(LightComponent)
    OWNER_COMPONENT_GETTER_DECL(ModelComponent)
    OWNER_COMPONENT_GETTER_DECL(SkyboxComponent)

  private:
    Component(const Component &);
    Component &operator=(const Component &);

    const ComponentType type_;
    Actor *owner_;
};

#endif
