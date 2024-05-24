#ifndef INCLUDED_ACTOR_H
#define INCLUDED_ACTOR_H

#include "component_factory.h"
#include "component_type.h"
#include "program.h"
#include <memory>
#include <string>

class TransformComponent;
class CameraComponent;
class LightComponent;
class ModelComponent;
class SkyboxComponent;
#define COMPONENT_GETTER(ENUM_TYPE_NAME, className)                                                                    \
    std::shared_ptr<className> Get##className()                                                                        \
    {                                                                                                                  \
        return std::reinterpret_pointer_cast<className>(GetComponent(ComponentType::ENUM_TYPE_NAME));                  \
    }

class RenderComponent;
class Component;
class Actor
{
  public:
    Actor(const std::string &name);
    virtual ~Actor();

    void Tick();
    void FinalTick();
    void Render(ProgramPtr program);

    uint32_t id() const
    {
        return id_;
    }

    void set_name(const std::string &name)
    {
        name_ = name;
    }
    std::string name() const
    {
        return name_;
    }

    void SetComponent(std::shared_ptr<Component> component);
    void RemoveComponent(ComponentType type);
    void AddCameraComponent();
    void AddTransformComponent();
    void AddLightComponent();
    void AddModelComponent();
    void AddSkyboxComponent();

    COMPONENT_GETTER(TRANSFORM, TransformComponent)
    COMPONENT_GETTER(CAMERA, CameraComponent)
    COMPONENT_GETTER(LIGHT, LightComponent)
    COMPONENT_GETTER(MODEL, ModelComponent)
    COMPONENT_GETTER(SKYBOX, SkyboxComponent)

  private:
    Actor(const Actor &c);
    Actor &operator=(const Actor &c);

    std::shared_ptr<Component> GetComponent(ComponentType type);

    static uint32_t s_id_;

    const uint32_t id_;
    std::string name_;
    std::shared_ptr<Component> components_[static_cast<int>(ComponentType::EOL)];
    std::shared_ptr<RenderComponent> render_component_;
};

#endif
