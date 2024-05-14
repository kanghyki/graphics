#ifndef INCLUDED_MESH_COMPONENT_H
#define INCLUDED_MESH_COMPONENT_H

#include "render_component.h"

class MeshComponent : public RenderComponent
{
  public:
    MeshComponent();
    ~MeshComponent();

    void Render() override;

  private:
    MeshComponent(const MeshComponent &);
    MeshComponent &operator=(const MeshComponent &);
};

#endif
