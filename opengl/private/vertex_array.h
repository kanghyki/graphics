#ifndef INCLUDED_VERTEX_ARRAY_H
#define INCLUDED_VERTEX_ARRAY_H

#include "libopengl.h"
#include "ptr.h"

class VertexArray
{
  public:
    static std::unique_ptr<VertexArray> Create();
    ~VertexArray();

    void Bind() const;
    void SetAttrib(uint32_t attrib_index, int count, uint32_t type, bool normalized, size_t stride,
                   uint64_t offset) const;

    inline uint32_t id() const
    {
        return id_;
    }

  private:
    VertexArray();
    VertexArray(const VertexArray &);
    VertexArray &operator=(const VertexArray &);

    void Init();

    uint32_t id_;
};

#endif
