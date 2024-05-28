#ifndef INCLUDED_LIGHT_COMPONENT_H
#define INCLUDED_LIGHT_COMPONENT_H

#include "camera.h"
#include "component.h"
#include "framebuffer.h"
#include "libmath.h"
#include "transform.h"
#include "uniform_struct.h"

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

    bool use_shadow() const
    {
        return use_shadow_;
    }
    void set_use_shadow(bool b)
    {
        use_shadow_ = true;
        depthmap_ = DepthMap::Create(1024, 1024);
    }
    DepthMapPtr depth_map()
    {
        return depthmap_;
    }

    LightData ToData() const
    {
        LightData data;
        memset(&data, 0, sizeof(LightData));

        data.type = static_cast<int>(type_);
        data.falloff_start = falloff_start_;
        data.falloff_end = falloff_end_;
        data.spot_power = spot_power_;

        data.position = position();
        data.color = color_;
        data.strength = strength_;
        data.direction = direction();

        return data;
    }

    glm::vec3 position() const
    {
        return transform_.position_;
    }
    glm::vec3 direction() const
    {
        return transform_.direction();
    }
    LightType type() const
    {
        return type_;
    }
    void set_type(LightType type)
    {
        type_ = type;
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

  private:
    LightComponent(const LightComponent &);
    LightComponent &operator=(const LightComponent &);

    void Tick() override;

    Transform transform_;

    LightType type_{LightType::DIRECTIONAL};
    glm::vec3 color_{1.0f, 1.0f, 1.0f};
    float strength_{1.0f};
    float spot_power_{1.0f};
    float falloff_start_{0.0f};
    float falloff_end_{10.0f};

    /* For shadow */
    bool use_shadow_{false};
    DepthMapPtr depthmap_{nullptr};
};

#endif
