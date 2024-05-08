#ifndef INCLUDED_FRAMEBUFFER_H
#define INCLUDED_FRAMEBUFFER_H

#include "ptr.h"
#include "texture.h"

#include <vector>

class BaseFramebuffer
{
  public:
    static void BindToDefault(uint32_t target = GL_FRAMEBUFFER);

    BaseFramebuffer();
    virtual ~BaseFramebuffer();

    bool Init();
    void Bind(uint32_t target = GL_FRAMEBUFFER);

    inline uint32_t id() const
    {
        return id_;
    }

  protected:
    virtual void AttachTexture() = 0;

  private:
    BaseFramebuffer(const BaseFramebuffer &);
    BaseFramebuffer &operator=(const BaseFramebuffer &);

    uint32_t id_;
};

CLASS_PTR(Framebuffer);
class Framebuffer : public BaseFramebuffer
{
  public:
    static FramebufferPtr Create(const std::vector<Texture2dPtr> color_attachments);
    ~Framebuffer();

    const Texture2dPtr color_attachment(int i) const
    {
        if (i >= 0 && i < color_attachments_.size())
        {
            return color_attachments_[i];
        }

        return nullptr;
    };

  private:
    Framebuffer();

    virtual void AttachTexture() override;

    void set_color_attachments(const std::vector<Texture2dPtr> &color_attachments)
    {
        color_attachments_ = color_attachments;
    }

    uint32_t depth_stencil_buffer_;
    std::vector<Texture2dPtr> color_attachments_;
};

#endif
