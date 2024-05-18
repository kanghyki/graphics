#include "camera_component.h"
#include "component_factory.h"
#include "light_component.h"
#include "mesh_component.h"
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
    case ComponentType::MESH:
        return std::shared_ptr<Component>(new MeshComponent());
    default:
        return nullptr;
    }
}
