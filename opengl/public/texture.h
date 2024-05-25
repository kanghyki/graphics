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

CLASS_PTR(BaseTexture);
class BaseTexture
{
  public:
    BaseTexture(uint32_t texture_type);
    virtual ~BaseTexture();

    void Bind() const;

    void SetFilter(uint32_t min_filter, uint32_t mag_filter) const;
    void SetWrap(uint32_t s_wrap, uint32_t t_wrap, uint32_t r_wrap = GL_NONE) const;
    void SetBorderColor(const glm::vec4 &color) const;

    const uint32_t id() const
    {
        return id_;
    }
    const uint32_t texture_type() const
    {
        return texture_type_;
    }
    uint32_t internal_format() const
    {
        return internal_format_;
    }
    uint32_t format() const;
    uint32_t data_type() const;

    int width() const
    {
        return width_;
    }
    int height() const
    {
        return height_;
    }

  protected:
    uint32_t id_{0};
    uint32_t internal_format_{0};
    uint32_t texture_type_{0};

    int width_{0};
    int height_{0};
};

CLASS_PTR(Texture);
class Texture : public BaseTexture
{
  public:
    static TexturePtr Load(const std::string &filename);
    static TexturePtr Create(const Image *image);
    static TexturePtr Create(int width, int height, uint32_t internal_format);
    ~Texture();

    void SetTextureFormat(int width, int height, uint32_t internal_format);

    std::array<uint8_t, 4> GetTexPixel(int x, int y) const;
    bool SaveAsPng(const std::string &filename, bool flip_vertical = true) const;

  private:
    Texture();

    void SetTextureFromImage(const Image *image);
};

CLASS_PTR(CubeTexture);
class CubeTexture : public BaseTexture
{
  public:
    static CubeTexturePtr Create(const std::vector<Image *> &images);
    static CubeTexturePtr Create(int width, int height, int length, uint32_t internal_format);
    ~CubeTexture();

    int length() const
    {
        return length_;
    }

  private:
    CubeTexture();

    void SetCubemapFromImages(const std::vector<Image *> &images);
    void SetCubemapFormat(int width, int height, int length, uint32_t internal_format);

    int length_{0};
};

#endif
