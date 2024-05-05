#ifndef INCLUDED_BUFFER_H
#define INCLUDED_BUFFER_H

#include "ptr.h"

CLASS_PTR(Buffer);
class Buffer
{
  public:
    ~Buffer();
    static BufferPtr Create(uint32_t buffer_type, uint32_t usage, const void *data, size_t stride, size_t count);

    void Bind() const;

    inline const uint32_t id() const
    {
        return id_;
    }
    inline size_t stride() const
    {
        return stride_;
    }
    inline size_t count() const
    {
        return count_;
    }

  private:
    Buffer();
    Buffer(const Buffer &);
    Buffer &operator=(const Buffer &);

    void Init(uint32_t buffer_type, uint32_t usage, const void *data, size_t stride, size_t count);

    uint32_t id_;
    uint32_t buffer_type_;
    uint32_t usage_;
    size_t stride_;
    size_t count_;
};

#endif
