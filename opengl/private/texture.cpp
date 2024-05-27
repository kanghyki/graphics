#include "texture.h"
#include <spdlog/spdlog.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

GLenum image_util::ChannelCountToRGBAFormat(uint32_t channel_count)
{
    GLenum format = GL_RGBA;

    switch (channel_count)
    {
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    default:
        break;
    }

    return format;
}

uint32_t image_util::RGBAFormatToChannelCount(GLenum format)
{
    int channel_count = 4;

    switch (format)
    {
    case GL_RED:
        channel_count = 1;
        break;
    case GL_RG:
        channel_count = 2;
        break;
    case GL_RGB:
        channel_count = 3;
        break;
    default:
        break;
    }

    return channel_count;
}

/*
 * BaseTexture
 */

BaseTexture::BaseTexture(uint32_t texture_type) : id_(0), texture_type_(texture_type)
{
    glGenTextures(1, &id_);
}

BaseTexture::~BaseTexture()
{
    if (id_)
    {
        glDeleteTextures(1, &id_);
    }
}

void BaseTexture::Bind() const
{
    glBindTexture(texture_type_, id_);
}

void BaseTexture::SetFilter(uint32_t min_filter, uint32_t mag_filter) const
{
    glTexParameteri(texture_type_, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(texture_type_, GL_TEXTURE_MAG_FILTER, mag_filter);
}

void BaseTexture::SetWrap(uint32_t s_wrap, uint32_t t_wrap, uint32_t r_wrap) const
{
    glTexParameteri(texture_type_, GL_TEXTURE_WRAP_S, s_wrap);
    glTexParameteri(texture_type_, GL_TEXTURE_WRAP_T, t_wrap);
    if (r_wrap != GL_NONE)
    {
        glTexParameteri(texture_type_, GL_TEXTURE_WRAP_R, r_wrap);
    }
}

void BaseTexture::SetBorderColor(const glm::vec4 &color) const
{
    glTexParameterfv(texture_type_, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
}

uint32_t BaseTexture::format() const
{
    switch (internal_format_)
    {
    case GL_DEPTH_COMPONENT:
        return GL_DEPTH_COMPONENT;
    case GL_RED:
    case GL_R:
    case GL_R16F:
    case GL_R32F:
        return GL_RED;
    case GL_RG:
    case GL_RG16F:
    case GL_RG32F:
        return GL_RG;
    case GL_RGB:
    case GL_RGB16F:
    case GL_RGB32F:
        return GL_RGB;
    case GL_RGBA:
    case GL_RGBA16F:
    case GL_RGBA32F:
        return GL_RGBA;
    }
    return -1;
}

uint32_t BaseTexture::data_type() const
{
    switch (internal_format_)
    {
    case GL_R16F:
    case GL_R32F:
    case GL_RG16F:
    case GL_RG32F:
    case GL_RGB16F:
    case GL_RGB32F:
    case GL_RGBA16F:
    case GL_RGBA32F:
    case GL_DEPTH_COMPONENT:
        return GL_FLOAT;
    case GL_RED:
    case GL_R:
    case GL_RG:
    case GL_RGB:
    case GL_RGBA:
        return GL_UNSIGNED_BYTE;
    }
    return -1;
}

/*
 * Texture
 */

Texture::Texture() : BaseTexture(GL_TEXTURE_2D)
{
}

Texture::~Texture()
{
}

TexturePtr Texture::Load(const std::string &filename)
{
    auto image = Image::Load(filename);
    if (!image)
    {
        return nullptr;
    }
    SPDLOG_INFO("image: {}x{}, {} channels", image->width(), image->height(), image->channel_count());
    auto texture = TexturePtr(new Texture());
    texture->Bind();
    texture->SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    texture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture->SetTextureFromImage(image.get());

    return texture;
}

TexturePtr Texture::Create(const Image *image)
{
    auto texture = TexturePtr(new Texture());
    texture->Bind();
    texture->SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    texture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture->SetTextureFromImage(image);

    return texture;
}

TexturePtr Texture::Create(int width, int height, uint32_t format)
{
    auto texture = TexturePtr(new Texture());
    texture->Bind();
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    texture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture->SetTextureFormat(width, height, format);

    return texture;
}

void Texture::SetTextureFromImage(const Image *image)
{
    width_ = image->width();
    height_ = image->height();
    internal_format_ = image_util::ChannelCountToRGBAFormat(image->channel_count());

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, width_, height_, 0, format(), data_type(), image->data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::SetTextureFormat(int width, int height, uint32_t internal_format)
{
    width_ = width;
    height_ = height;
    internal_format_ = internal_format;

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, width_, height_, 0, format(), data_type(), nullptr);
}

std::array<uint8_t, 4> Texture::GetTexPixel(int x, int y) const
{
    uint8_t pixel[4];

    Bind();
    glReadPixels(x, y, 1, 1, format(), GL_UNSIGNED_BYTE, pixel);

    return {pixel[0], pixel[1], pixel[2], pixel[3]};
}

bool Texture::SaveAsPng(const std::string &filename, bool flip_vertical) const
{
    int channel_count = image_util::RGBAFormatToChannelCount(format());
    unsigned char *data = new unsigned char[width_ * height_ * channel_count];
    if (!data)
    {
        SPDLOG_ERROR("malloc error");
        return false;
    }
    Bind();
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    bool result = true;
    stbi_flip_vertically_on_write(flip_vertical);
    if (!stbi_write_png(filename.c_str(), width_, height_, channel_count, data, width_ * channel_count))
    {
        SPDLOG_ERROR("failed to save");
        result = false;
    }
    delete[] data;

    return result;
}

/*
 * CubeTexture
 */

CubeTexture::CubeTexture() : BaseTexture(GL_TEXTURE_CUBE_MAP)
{
}

CubeTexture::~CubeTexture()
{
}

CubeTexturePtr CubeTexture::Create(const std::vector<Image *> &images)
{
    auto texture = CubeTexturePtr(new CubeTexture());

    if (images.size() != 6)
    {
        return nullptr;
    }
    texture->Bind();
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    texture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture->SetCubemapFromImages(images);

    return std::move(texture);
}

CubeTexturePtr CubeTexture::Create(int width, int height, int length, uint32_t internal_format)
{
    auto texture = CubeTexturePtr(new CubeTexture());

    texture->Bind();
    texture->SetFilter(GL_LINEAR, GL_LINEAR);
    texture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture->SetCubemapFormat(width, height, length, internal_format);

    return std::move(texture);
}

void CubeTexture::SetCubemapFromImages(const std::vector<Image *> &images)
{
    // { right, left, top, bottom, front, back }
    width_ = images[4]->width();
    height_ = images[4]->height();
    length_ = images[0]->width();
    internal_format_ = image_util::ChannelCountToRGBAFormat(images[0]->channel_count());

    for (uint32_t i = 0; i < (uint32_t)images.size(); i++)
    {
        Image *image = images[i];
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image->width(), image->height(), 0, format(),
                     data_type(), image->data());
    }
}

void CubeTexture::SetCubemapFormat(int width, int height, int length, uint32_t internal_format)
{
    width_ = width;
    height_ = height;
    length_ = length;
    internal_format_ = internal_format;

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internal_format_, height_, length_, 0, format(), data_type(), NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internal_format_, height_, length_, 0, format(), data_type(), NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internal_format_, width_, height_, 0, format(), data_type(), NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internal_format_, width_, height_, 0, format(), data_type(), NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internal_format_, length_, width_, 0, format(), data_type(), NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internal_format_, length_, width_, 0, format(), data_type(), NULL);
}