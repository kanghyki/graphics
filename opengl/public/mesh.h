#ifndef INCLUDED_MESH_H
#define INCLUDED_MESH_H

#include "buffer.h"
#include "material.h"
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

    void Draw(const Program *program) const;

    inline const VertexArray *vertex_array() const
    {
        return vertex_array_.get();
    }
    inline BufferPtr vertex_buffer() const
    {
        return vertex_buffer_;
    }
    inline BufferPtr index_buffer() const
    {
        return index_buffer_;
    }
    inline MaterialPtr material() const
    {
        return material_;
    }
    inline void set_material(MaterialPtr material)
    {
        material_ = material;
    }

  private:
    Mesh(uint32_t primitive_type);
    Mesh(const Mesh &mesh);

    void Init(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);

    uint32_t primitive_type_{GL_TRIANGLES};
    std::unique_ptr<VertexArray> vertex_array_{nullptr};
    BufferPtr vertex_buffer_{nullptr};
    BufferPtr index_buffer_{nullptr};
    MaterialPtr material_{nullptr};
};

#endif
