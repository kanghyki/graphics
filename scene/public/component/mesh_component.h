#ifndef INCLUDED_MESH_COMPONENT_H
#define INCLUDED_MESH_COMPONENT_H

#include "component.h"

class MeshComponent : public Component
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
