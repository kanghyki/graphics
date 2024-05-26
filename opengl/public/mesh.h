#ifndef INCLUDED_MESH_H
#define INCLUDED_MESH_H

#include "buffer.h"
#include "libmath.h"
#include "material.h"
#include "program.h"
#include "ptr.h"
#include "vertex_array.h"
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent;
};

CLASS_PTR(Mesh);
class Mesh
{
  public:
    static MeshPtr Create(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices,
                          uint32_t primitive_type);
    static MeshPtr CreateBox();
    static MeshPtr CreateSphere(uint32_t slice, uint32_t stack);
    static MeshPtr CreatePlane();
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
