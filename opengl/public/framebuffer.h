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
    static FramebufferPtr Create(const std::vector<BaseTexturePtr> color_attachments);
    ~Framebuffer();

    const BaseTexture *color_attachment(int i) const
    {
        if (i >= 0 && i < color_attachments_.size())
        {
            return color_attachments_[i].get();
        }

        return nullptr;
    };

  private:
    Framebuffer();

    void set_color_attachments(const std::vector<BaseTexturePtr> &color_attachments)
    {
        color_attachments_ = color_attachments;
    }
    void AttachTexture() override;

    uint32_t depth_stencil_buffer_;
    std::vector<BaseTexturePtr> color_attachments_;
};

CLASS_PTR(DepthMap);
class DepthMap : public BaseFramebuffer
{
  public:
    static DepthMapPtr Create(int width, int height, int length = -1);
    static DepthMap *empty()
    {
        static DepthMapPtr d = nullptr;
        if (!d)
        {
            d = DepthMap::Create(1, 1);
        }
        return d.get();
    }
    static DepthMap *empty_cube()
    {
        static DepthMapPtr d = nullptr;
        if (!d)
        {
            d = DepthMap::Create(1, 1, 1);
        }
        return d.get();
    }
    ~DepthMap();

    BaseTexture *texture()
    {
        return texture_.get();
    }

  private:
    DepthMap();

    bool SetTexture(int width, int height, int length);
    void AttachTexture() override;

    BaseTexturePtr texture_;
};

#endif
