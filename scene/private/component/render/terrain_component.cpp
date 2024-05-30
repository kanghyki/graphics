#include "terrain_component.h"
#include "renderer.h"
#include "transform_component.h"

TerrainComponent::TerrainComponent() : RenderComponent(ComponentType::TERRAIN)
{
}

TerrainComponent::~TerrainComponent()
{
}

void TerrainComponent::Render(const Program *program)
{
    if (height_map_)
    {
        glActiveTexture(GL_TEXTURE0);
        height_map_->Bind();
        program->SetUniform("height_map", 0);
        program->SetUniform("height_scale", height_scale_);
        program->SetUniform("model", GetTransformComponent()->CalcModelMatrix());
        program->SetUniform("height_map_width", height_map_->width());
        program->SetUniform("height_map_height", height_map_->height());
        vao_->Bind();
        glDrawArrays(GL_PATCHES, 0, patches_ * patch_size_ * patch_size_);
    }
}

void TerrainComponent::GenerateMesh(int width, int height)
{
    std::vector<float> vertices;
    vertices.reserve(patch_size_ * patch_size_ * 20);
    float patch_size_f = static_cast<float>(patch_size_);
    for (int i = 0; i < patch_size_; ++i)
    {
        for (int j = 0; j < patch_size_; ++j)
        {
            vertices.push_back(-width / 2.0f + width * i / patch_size_f);
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + height * j / patch_size_f);
            vertices.push_back(i / patch_size_f);
            vertices.push_back(j / patch_size_f);

            vertices.push_back(-width / 2.0f + width * (i + 1) / patch_size_f);
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + height * j / patch_size_f);
            vertices.push_back((i + 1) / patch_size_f);
            vertices.push_back(j / patch_size_f);

            vertices.push_back(-width / 2.0f + width * i / patch_size_f);
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + height * (j + 1) / patch_size_f);
            vertices.push_back(i / patch_size_f);
            vertices.push_back((j + 1) / patch_size_f);

            vertices.push_back(-width / 2.0f + width * (i + 1) / patch_size_f);
            vertices.push_back(0.0f);
            vertices.push_back(-height / 2.0f + height * (j + 1) / patch_size_f);
            vertices.push_back((i + 1) / patch_size_f);
            vertices.push_back((j + 1) / patch_size_f);
        }
    }
    vao_ = VertexArray::Create();
    vbo_ = Buffer::Create(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(float), vertices.size());
    vao_->SetAttrib(0, 3, GL_FLOAT, false, sizeof(float) * 5, 0);
    vao_->SetAttrib(1, 2, GL_FLOAT, false, sizeof(float) * 5, sizeof(float) * 3);
    glPatchParameteri(GL_PATCH_VERTICES, patches_);
}