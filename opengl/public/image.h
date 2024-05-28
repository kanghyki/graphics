#ifndef INCLUDED_IMAGE_H
#define INCLUDED_IMAGE_H

#include "libmath.h"
#include "ptr.h"
#include <string>

CLASS_PTR(Image);
class Image
{
  public:
    static ImagePtr Load(const std::string &filepath, bool flip_vertical = true);
    static ImagePtr Create(int width, int height, int channel_count = 4, int byte_per_channel = 1);
    static ImagePtr CreateSingleColorImage(int width, int height, const glm::vec4 &color);
    ~Image();

    const uint8_t *data() const
    {
        return data_;
    };
    int width() const
    {
        return width_;
    };
    int height() const
    {
        return height_;
    };
    int channel_count() const
    {
        return channel_count_;
    };
    int byte_per_channel() const
    {
        return byte_per_channel_;
    }

  private:
    Image();
    bool LoadFile(const std::string &filepath, bool flip_vertical);
    bool Allocate(int width, int height, int channel_count, int byte_per_channel);

    int width_{0};
    int height_{0};
    int channel_count_{0};
    int byte_per_channel_{1};
    uint8_t *data_{nullptr};
};

#endif
