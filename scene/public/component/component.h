#ifndef INCLUDED_COMPONENT_H
#define INCLUDED_COMPONENT_H

#include "actor.h"
#include "component_type.h"

#define OWNER_COMPONENT_GETTER(className)                                                                              \
    className *Get##className()                                                                                        \
    {                                                                                                                  \
        return owner_->Get##className();                                                                               \
    }

class Component
{
    friend class Actor;

  public:
    Component(ComponentType type);
    virtual ~Component();

    virtual void Tick();
    virtual void Render();

    ComponentType type()
    {
        return type_;
    }

    OWNER_COMPONENT_GETTER(TransformComponent)
    OWNER_COMPONENT_GETTER(CameraComponent)
    OWNER_COMPONENT_GETTER(LightComponent)
    OWNER_COMPONENT_GETTER(MeshComponent)
    OWNER_COMPONENT_GETTER(SkyboxComponent)
    OWNER_COMPONENT_GETTER(LandscapeComponent)

  private:
    Component(const Component &);
    Component &operator=(const Component &);

    const ComponentType type_;
    Actor *owner_;
};

#endif
