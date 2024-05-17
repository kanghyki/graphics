#ifndef INCLUDED_MESH_H
#define INCLUDED_MESH_H

#include "buffer.h"
#include "libmath.h"
#include "ptr.h"
#include "vertex_array.h"
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;
    glm::vec3 tangent;
};

CLASS_PTR(Mesh);
class Mesh
{
  public:
    static MeshPtr Create(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices,
                          uint32_t primitive_type);
    static MeshPtr CreateBox();
    static MeshPtr CreateSphere(size_t slice, size_t stack);
    static MeshPtr CreatePlane();
    static void ComputeTangents(std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
    ~Mesh();

    void Draw() const;

  private:
    Mesh(uint32_t primitive_type);
    Mesh(const Mesh &);
    Mesh &operator=(const Mesh &);

    void Init(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

    uint32_t primitive_type_;
    VertexArrayUPtr vertex_array_;
    BufferPtr vertex_buffer_;
    BufferPtr index_buffer_;
};

#endif
