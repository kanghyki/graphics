#ifndef INCLUDED_MESH_COMPONENT_H
#define INCLUDED_MESH_COMPONENT_H

#include "material.h"
#include "mesh.h"
#include "render_component.h"

class MeshComponent : public RenderComponent
{
  public:
    MeshComponent();
    ~MeshComponent();

    void Render(ProgramPtr program) override;

    MeshPtr mesh()
    {
        return mesh_;
    }
    void set_mesh(const MeshPtr &mesh)
    {
        mesh_ = mesh;
    }

  private:
    MeshComponent(const MeshComponent &);
    MeshComponent &operator=(const MeshComponent &);

    MeshPtr mesh_;
};

#endif
