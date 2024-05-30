#include "mesh.h"

Mesh::Mesh(uint32_t primitive_type)
    : primitive_type_(primitive_type), vertex_array_(nullptr), vertex_buffer_(nullptr), index_buffer_(nullptr),
      material_(nullptr)
{
}

Mesh::~Mesh()
{
}

MeshPtr Mesh::Create(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, uint32_t primitive_type)
{
    auto mesh = MeshPtr(new Mesh(primitive_type));
    mesh->Init(vertices, indices);

    return mesh;
}

MeshPtr Mesh::Create(MeshGeometry::Data data, uint32_t primitive_type)
{
    auto mesh = MeshPtr(new Mesh(primitive_type));
    mesh->Init(data.vertices, data.indices);

    return mesh;
}

void Mesh::ComputeTangents(std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
{
    auto compute = [](const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::vec3 &pos3, const glm::vec2 &uv1,
                      const glm::vec2 &uv2, const glm::vec2 &uv3) -> glm::vec3 {
        auto edge1 = pos2 - pos1;
        auto edge2 = pos3 - pos1;
        auto deltaUV1 = uv2 - uv1;
        auto deltaUV2 = uv3 - uv1;
        float det = (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        if (det != 0.0f)
        {
            auto invDet = 1.0f / det;
            return deltaUV2.y * edge1 - deltaUV1.y * edge2;
        }
        else
        {
            return glm::vec3(0.0f, 0.0f, 0.0f);
        }
    };

    std::vector<glm::vec3> tangents(vertices.size());
    memset(tangents.data(), 0, tangents.size() * sizeof(glm::vec3));
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        auto i0 = indices[i];
        auto i1 = indices[i + 1];
        auto i2 = indices[i + 2];

        tangents[i0] += compute(vertices[i0].position, vertices[i1].position, vertices[i2].position, vertices[i0].uv,
                                vertices[i1].uv, vertices[i2].uv);

        tangents[i1] = compute(vertices[i1].position, vertices[i2].position, vertices[i0].position, vertices[i1].uv,
                               vertices[i2].uv, vertices[i0].uv);

        tangents[i2] = compute(vertices[i2].position, vertices[i0].position, vertices[i1].position, vertices[i2].uv,
                               vertices[i0].uv, vertices[i1].uv);
    }

    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i].tangent = glm::normalize(tangents[i]);
    }
}

void Mesh::Init(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
{
    if (primitive_type_ == GL_TRIANGLES)
    {
        ComputeTangents(const_cast<std::vector<Vertex> &>(vertices), indices);
    }
    vertex_array_ = VertexArray::Create();
    vertex_buffer_ = Buffer::Create(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(Vertex), vertices.size());
    index_buffer_ =
        Buffer::Create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t), indices.size());
    vertex_array_->SetAttrib(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
    vertex_array_->SetAttrib(1, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normal));
    vertex_array_->SetAttrib(2, 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, uv));
    vertex_array_->SetAttrib(3, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, tangent));
}

void Mesh::Draw(const Program *program) const
{
    vertex_array_->Bind();
    if (program && material_)
    {
        material_->Setup(program);
    }
    glDrawElements(primitive_type_, index_buffer_->count(), GL_UNSIGNED_INT, 0);
}
