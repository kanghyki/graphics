#ifndef INCLUDED_LIGHT_COMPONENT_H
#define INCLUDED_LIGHT_COMPONENT_H

#include "component.h"
#include "framebuffer.h"
#include "libmath.h"
#include "transform.h"
#include "uniform_struct.h"
#include <spdlog/spdlog.h>

enum class LightType
{
    DIRECTIONAL = 0,
    POINT,
    SPOT
};

class LightComponent : public Component
{
  public:
    LightComponent();
    ~LightComponent();

    LightData to_data() const;

    glm::vec3 position() const;

    glm::vec3 direction() const;

    LightType type() const
    {
        return type_;
    }
    void set_type(LightType type)
    {
        type_ = type;
        if (use_shadow_)
        {
            set_use_shadow(false);
            set_use_shadow(true);
        }
    }

    glm::vec3 color() const
    {
        return color_;
    }
    void set_color(const glm::vec3 &color)
    {
        color_ = color;
    }

    float strength() const
    {
        return strength_;
    }
    void set_strength(float strength)
    {
        strength_ = strength;
    }

    float spot_power() const
    {
        return spot_power_;
    }
    void set_spot_power(float spot_power)
    {
        spot_power_ = spot_power;
    }

    float falloff_start() const
    {
        return falloff_start_;
    }
    void set_falloff_start(float falloff_start)
    {
        falloff_start_ = falloff_start;
    }

    float falloff_end() const
    {
        return falloff_end_;
    }
    void set_falloff_end(float falloff_end)
    {
        falloff_end_ = falloff_end;
    }

    /*





    */
    bool use_shadow() const
    {
        return use_shadow_;
    }

    void set_use_shadow(bool b)
    {
        use_shadow_ = b;
        if (!b)
        {
            depth_map_ = nullptr;
            return;
        }
        switch (type_)
        {
        case LightType::DIRECTIONAL:
            depth_map_ = DepthMap::Create(TextureArray::Create(1280, 1280, 5, GL_DEPTH_COMPONENT));
            break;
        case LightType::SPOT:
            depth_map_ = DepthMap::Create(Texture::Create(1280, 1280, GL_DEPTH_COMPONENT));
            break;
        case LightType::POINT:
            depth_map_ = DepthMap::Create(CubeTexture::Create(1280, 1280, 1280, GL_DEPTH_COMPONENT));
            break;
        default:
            break;
        }
    }

    DepthMap *depth_map()
    {
        return depth_map_.get();
    }

    // Spot, Point Only
    uint32_t shadow_id() const
    {
        return shadow_id_;
    }

    // Spot, Point Only
    void set_shadow_id(uint32_t id)
    {
        shadow_id_ = id;
    }

    // Spot, Point Only
    float near_plane() const
    {
        return 0.1f;
    }

    // Spot, Point Only
    float far_plane() const
    {
        return falloff_end_ * 10.0f;
    }

    // Spot Light only
    glm::mat4 CalcView() const
    {
        glm::mat4 rr = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        return glm::lookAt(position(), position() + direction(), glm::vec3(rr * glm::vec4(direction(), 1.0f)));
    }

    // Spot Light only
    glm::mat4 CalcProj() const
    {
        return glm::perspective(glm::radians(120.0f), 1.0f, near_plane(), far_plane());
    }

    mutable float camera_far_plane_;
    mutable std::vector<float> cascade_level;
    mutable std::vector<glm::mat4> matrices_;
    std::vector<float> CalcCascadeLevel(const float far_plane) const
    {
        return {far_plane / 50.0f, far_plane / 25.0f, far_plane / 10.0f, far_plane / 2.0f};
    }

    std::vector<glm::mat4> CalcLightPVMatrices(const float fov_y, const float aspect, const float near_plane,
                                               const float far_plane, const glm::mat4 &view) const
    {
        matrices_.clear();
        camera_far_plane_ = far_plane;
        cascade_level = CalcCascadeLevel(far_plane);

        for (size_t i = 0; i < cascade_level.size() + 1; ++i)
        {
            float np;
            float fp;
            if (i == 0)
            {
                np = near_plane;
                fp = cascade_level[i];
            }
            else if (i < cascade_level.size())
            {
                np = cascade_level[i - i];
                fp = cascade_level[i];
            }
            else
            {
                np = cascade_level[i - i];
                fp = far_plane;
            }
            const glm::mat4 proj = glm::perspective(glm::radians(fov_y), aspect, np, fp);
            matrices_.push_back(CalcLightPVMatrix(proj, view));
        }

        return matrices_;
    }

  private:
    LightComponent(const LightComponent &);
    LightComponent &operator=(const LightComponent &);

    void Tick() override;

    LightType type_{LightType::DIRECTIONAL};
    glm::vec3 color_{1.0f, 1.0f, 1.0f};
    float strength_{1.0f};
    float spot_power_{1.0f};
    float falloff_start_{0.0f};
    float falloff_end_{10.0f};

    bool use_shadow_{false};
    DepthMapPtr depth_map_{nullptr};
    uint32_t shadow_id_{0};

    std::vector<glm::vec4> GetFrustumCorners(const glm::mat4 &proj, const glm::mat4 &view) const
    {
        const glm::mat4 inv = glm::inverse(proj * view);
        std::vector<glm::vec4> frustum_corners;
        frustum_corners.reserve(8);
        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                    frustum_corners.push_back(pt / pt.w);
                }
            }
        }
        return frustum_corners;
    }

    glm::mat4 CalcLightPVMatrix(const glm::mat4 &proj, const glm::mat4 &view) const
    {
        const auto corners = GetFrustumCorners(proj, view);
        glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
        for (const auto &corner : corners)
        {
            center += glm::vec3(corner);
        }
        center /= corners.size();

        glm::vec3 dir = direction();
        glm::mat4 rr = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 light_view = glm::lookAt(center, center + dir, glm::vec3(rr * glm::vec4(dir, 1.0f)));

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();
        for (const auto &corner : corners)
        {
            const auto trf = light_view * corner;
            minX = std::min(minX, trf.x);
            maxX = std::max(maxX, trf.x);
            minY = std::min(minY, trf.y);
            maxY = std::max(maxY, trf.y);
            minZ = std::min(minZ, trf.z);
            maxZ = std::max(maxZ, trf.z);
        }

        constexpr float zMult = 10.0f;
        minZ *= minZ < 0 ? zMult : 1 / zMult;
        maxZ *= maxZ < 0 ? 1 / zMult : zMult;
        const glm::mat4 light_proj = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

        return light_proj * light_view;
    }
};

#endif
