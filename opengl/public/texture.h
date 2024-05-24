#ifndef INCLUDED_TEXTURE_H
#define INCLUDED_TEXTURE_H

#include "image.h"
#include "libopengl.h"
#include "ptr.h"
#include <vector>

namespace image_util
{
GLenum ChannelCountToRGBAFormat(uint32_t channel_count);
uint32_t RGBAFormatToChannelCount(GLenum format);
}; // namespace image_util

class BaseTexture
{
  public:
    BaseTexture(uint32_t texture_type);
    virtual ~BaseTexture();

    void Bind() const;

    void SetFilter(uint32_t min_filter, uint32_t mag_filter) const;
    void SetWrap(uint32_t s_wrap, uint32_t t_wrap, uint32_t r_wrap = GL_NONE) const;

    const uint32_t id() const
    {
        return id_;
    }
    const uint32_t texture_type() const
    {
        return texture_type_;
    }

  private:
    uint32_t id_;
    uint32_t texture_type_{GL_TEXTURE_2D};
};

CLASS_PTR(Texture);
class Texture : public BaseTexture
{
  public:
    static TexturePtr Load(const std::string &filename);
    static TexturePtr Create(const Image *image);
    static TexturePtr Create(int width, int height, uint32_t inner_format = GL_RGBA, uint32_t format = GL_RGBA,
                             uint32_t type = GL_UNSIGNED_BYTE);
    ~Texture();

    void SetTextureFormat(int width, int height, uint32_t inner_format, uint32_t format, uint32_t type);
    void SetBorderColor(const glm::vec4 &color) const;
    unsigned char *GetTexImage() const;
    std::array<uint8_t, 4> GetTexPixel(int x, int y) const;
    bool SaveAsPng(const std::string &filename) const;

    int width() const
    {
        return width_;
    }
    int height() const
    {
        return height_;
    }
    uint32_t inner_format() const
    {
        return inner_format_;
    }
    uint32_t format() const
    {
        return format_;
    }
    uint32_t type() const
    {
        return type_;
    }

  private:
    Texture();

    void SetTextureFromImage(const Image *image);

    int width_{0};
    int height_{0};
    uint32_t inner_format_{GL_RGBA};
    uint32_t format_{GL_RGBA};
    uint32_t type_{GL_UNSIGNED_BYTE};
};

CLASS_PTR(CubeTexture);
class CubeTexture : public BaseTexture
{
  public:
    static CubeTexturePtr Create(const std::vector<Image *> &images);
    static CubeTexturePtr Create(int width, int height, int length, uint32_t inner_format, uint32_t format,
                                 uint32_t type);
    ~CubeTexture();

    int width() const
    {
        return width_;
    }
    int height() const
    {
        return height_;
    }
    int length() const
    {
        return length_;
    }
    uint32_t inner_format() const
    {
        return inner_format_;
    }
    uint32_t type() const
    {
        return type_;
    }
    uint32_t format() const
    {
        return format_;
    }

  private:
    CubeTexture();

    void SetCubeMapFromImages(const std::vector<Image *> &images);
    void SetCubeMapFormat(int width, int height, int length, uint32_t inner_format, uint32_t format, uint32_t type);

    int width_{0};
    int height_{0};
    int length_{0};
    uint32_t inner_format_{GL_RGBA};
    uint32_t format_{GL_RGBA};
    uint32_t type_{GL_UNSIGNED_BYTE};
};

#endif
