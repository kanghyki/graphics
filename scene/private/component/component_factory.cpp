#include "component_factory.h"
#include "camera_component.h"
#include "light_component.h"
#include "model_component.h"
#include "skybox_component.h"
#include "terrain_component.h"
#include "transform_component.h"

ComponentFactory::ComponentFactory()
{
}
ComponentFactory::~ComponentFactory()
{
}

std::shared_ptr<Component> ComponentFactory::Generate(ComponentType type) const
{
    switch (type)
    {
    case ComponentType::CAMERA:
        return std::shared_ptr<Component>(new CameraComponent());
    case ComponentType::LIGHT:
        return std::shared_ptr<Component>(new LightComponent());
    case ComponentType::TRANSFORM:
        return std::shared_ptr<Component>(new TransformComponent());
    case ComponentType::MODEL:
        return std::shared_ptr<Component>(new ModelComponent());
    case ComponentType::SKYBOX:
        return std::shared_ptr<Component>(new SkyboxComponent());
    case ComponentType::TERRAIN:
        return std::shared_ptr<Component>(new TerrainComponent());
    default:
        return nullptr;
    }
}
