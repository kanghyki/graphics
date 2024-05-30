#ifndef INCLUDED_TERRAIN_COMPONENT_H
#define INCLUDED_TERRAIN_COMPONENT_H

#include "buffer.h"
#include "render_component.h"
#include "texture.h"
#include "vertex_array.h"

class TerrainComponent : public RenderComponent
{
  public:
    TerrainComponent();
    ~TerrainComponent();

    void Render(const Program *program) override;

    Texture *height_map()
    {
        return height_map_.get();
    }
    void set_height_map(TexturePtr height_map)
    {
        height_map_ = height_map;
        GenerateMesh(height_map_->width(), height_map_->height());
    }

    float height_scale()
    {
        return height_scale_;
    }
    void set_height_scale(float scale)
    {
        height_scale_ = scale;
    }

    Texture *texture()
    {
        return texture_.get();
    }
    void set_texture(TexturePtr texture)
    {
        texture_ = texture;
    }

  private:
    TerrainComponent(const TerrainComponent &rhs);
    TerrainComponent &operator=(const TerrainComponent &rhs);

    void GenerateMesh(int width, int height);

    TexturePtr height_map_;
    TexturePtr texture_;
    float height_scale_{1.0f};

    VertexArrayPtr vao_;
    BufferPtr vbo_;
    int patches_{4};
    int patch_size_{20};
};

#endif