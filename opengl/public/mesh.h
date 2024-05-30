#ifndef INCLUDED_MESH_H
#define INCLUDED_MESH_H

#include "buffer.h"
#include "material.h"
#include "mesh_geometry.h"
#include "program.h"
#include "ptr.h"
#include "vertex.h"
#include "vertex_array.h"
#include <vector>

CLASS_PTR(Mesh);
class Mesh
{
  public:
    static MeshPtr Create(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices,
                          uint32_t primitive_type = GL_TRIANGLES);
    static MeshPtr Create(MeshGeometry::Data data, uint32_t primitive_type = GL_TRIANGLES);
    static void ComputeTangents(std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
    ~Mesh();

    MaterialPtr material() const
    {
        return material_;
    }
    void set_material(MaterialPtr material)
    {
        material_ = material;
    }

    void Draw(const Program *program) const;

  private:
    Mesh(uint32_t primitive_type);
    Mesh(const Mesh &);
    Mesh &operator=(const Mesh &);

    void Init(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

    uint32_t primitive_type_;
    VertexArrayUPtr vertex_array_;
    BufferPtr vertex_buffer_;
    BufferPtr index_buffer_;
    MaterialPtr material_;
};

#endif
