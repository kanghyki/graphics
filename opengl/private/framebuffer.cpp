#include "framebuffer.h"
#include "libopengl.h"
#include <spdlog/spdlog.h>

void BaseFramebuffer::BindToDefault(uint32_t target)
{
    glBindFramebuffer(target, 0);
};

BaseFramebuffer::BaseFramebuffer() : id_(0)
{
}

BaseFramebuffer::~BaseFramebuffer()
{
    if (id_)
    {
        glDeleteFramebuffers(1, &id_);
    }
}

bool BaseFramebuffer::Init()
{
    bool ret = true;
    glGenFramebuffers(1, &id_);
    Bind();
    AttachTexture();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        SPDLOG_ERROR("failed to create framebuffer");
        ret = false;
    }
    BindToDefault();
    return ret;
}

void BaseFramebuffer::Bind(uint32_t target)
{
    glBindFramebuffer(target, id_);
}

/*
 * Framebuffer
 */

Framebuffer::Framebuffer() : BaseFramebuffer(), depth_stencil_buffer_(0), color_attachments_(0)
{
}

Framebuffer::~Framebuffer()
{
    if (depth_stencil_buffer_)
    {
        glDeleteRenderbuffers(1, &depth_stencil_buffer_);
    }
}

FramebufferPtr Framebuffer::Create(const std::vector<BaseTexturePtr> color_attachments)
{
    auto framebuffer = FramebufferPtr(new Framebuffer());

    framebuffer->set_color_attachments(color_attachments);
    if (!framebuffer->Init())
    {
        return nullptr;
    }

    return framebuffer;
}

void Framebuffer::AttachTexture()
{
    for (size_t i = 0; i < color_attachments_.size(); ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, color_attachments_[i]->id(), 0);
    }

    if (color_attachments_.size() > 0)
    {
        std::vector<GLenum> attachments(color_attachments_.size());
        for (size_t i = 0; i < color_attachments_.size(); ++i)
        {
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glDrawBuffers(color_attachments_.size(), attachments.data());

        // depth stencil buffer
        glGenRenderbuffers(1, &depth_stencil_buffer_);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_buffer_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, color_attachments_[0]->width(),
                              color_attachments_[0]->height());
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buffer_);
    }
}

/*
 * Depthmap
 */

DepthMapPtr DepthMap::Create(int width, int height, int length)
{
    auto depth_map = DepthMapPtr(new DepthMap());

    if (!depth_map->SetTexture(width, height, length) || !depth_map->Init())
    {
        return nullptr;
    }

    return depth_map;
}

DepthMap::DepthMap()
{
}

DepthMap::~DepthMap()
{
}

bool DepthMap::SetTexture(int width, int height, int length)
{
    length == -1 ? depth_texture_ = Texture::Create(width, height, GL_DEPTH_COMPONENT)
                 : depth_texture_ = CubeTexture::Create(width, height, length, GL_DEPTH_COMPONENT);
    if (!depth_texture_)
    {
        return false;
    }
    depth_texture_->SetFilter(GL_NEAREST, GL_NEAREST);
    depth_texture_->SetWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    depth_texture_->SetBorderColor(glm::vec4(1.0f));
    return true;
}

void DepthMap::AttachTexture()
{
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture_->id(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}