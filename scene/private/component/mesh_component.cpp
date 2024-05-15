#include "camera_component.h"
#include "mesh_component.h"
#include "renderer.h"

MeshComponent::MeshComponent() : RenderComponent(ComponentType::MESH)
{
}

MeshComponent::~MeshComponent()
{
}
