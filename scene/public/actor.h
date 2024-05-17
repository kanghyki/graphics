#ifndef INCLUDED_ACTOR_H
#define INCLUDED_ACTOR_H

#include "component_type.h"
#include "program.h"
#include <string>

class RenderComponent;
class TransformComponent;
class CameraComponent;
class LightComponent;
class MeshComponent;
class SkyboxComponent;
class LandscapeComponent;

#define COMPONENT_GETTER(ENUM_TYPE_NAME, className)                                                                    \
    className *Get##className()                                                                                        \
    {                                                                                                                  \
        return reinterpret_cast<className *>(GetComponent(ComponentType::ENUM_TYPE_NAME));                             \
    }

class Component;
class Actor
{
  public:
    Actor(const std::string &name);
    virtual ~Actor();

    void Tick();
    void FinalTick();
    void Render(ProgramPtr program);

    uint32_t id()
    {
        return id_;
    }

    void SetComponent(Component *component);
    Component *GetComponent(ComponentType type);

    COMPONENT_GETTER(TRANSFORM, TransformComponent)
    COMPONENT_GETTER(CAMERA, CameraComponent)
    COMPONENT_GETTER(LIGHT, LightComponent)
    COMPONENT_GETTER(MESH, MeshComponent)
    COMPONENT_GETTER(SKY_BOX, SkyboxComponent)
    COMPONENT_GETTER(LAND_SCAPE, LandscapeComponent)

  private:
    Actor(const Actor &c);
    Actor &operator=(const Actor &c);

    static uint32_t s_id_;

    const uint32_t id_;
    std::string name_;
    Component *components_[static_cast<int>(ComponentType::EOL)];
    RenderComponent *render_component_;
};

#endif
