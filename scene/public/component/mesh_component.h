#ifndef INCLUDED_MESH_COMPONENT_H
#define INCLUDED_MESH_COMPONENT_H

#include "model.h"
#include "render_component.h"

class MeshComponent : public RenderComponent
{
  public:
    MeshComponent();
    ~MeshComponent();

  private:
    MeshComponent(const MeshComponent &);
    MeshComponent &operator=(const MeshComponent &);
};

#endif
