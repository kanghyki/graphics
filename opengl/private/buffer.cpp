#include "buffer.h"
#include "libopengl.h"

Buffer::Buffer() : id_(0), buffer_type_(0), usage_(0), stride_(0), count_(0)
{
}

Buffer::~Buffer()
{
    if (id_)
    {
        glDeleteBuffers(1, &id_);
    }
}

BufferPtr Buffer::Create(uint32_t buffer_type, uint32_t usage, const void *data, size_t stride, size_t count)
{
    auto buffer = BufferPtr(new Buffer());
    buffer->Init(buffer_type, usage, data, stride, count);

    return buffer;
}

void Buffer::Bind() const
{
    glBindBuffer(buffer_type_, id_);
}

void Buffer::Init(uint32_t buffer_type, uint32_t usage, const void *data, size_t stride, size_t count)
{
    buffer_type_ = buffer_type;
    usage_ = usage;
    stride_ = stride;
    count_ = count;
    glGenBuffers(1, &id_);
    Bind();
    glBufferData(buffer_type_, stride_ * count_, data, usage);
}
