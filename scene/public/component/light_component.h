#ifndef INCLUDED_LIGHT_COMPONENT_H
#define INCLUDED_LIGHT_COMPONENT_H

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

        data.use_shadow = use_shadow_;
        if (use_shadow_)
        {
            data.shadow_id = shadow_id_;
            data.view = CalcView();
            data.proj = CalcProj();
        }

        return data;
    }

    glm::mat4 CalcView() const
    {
        glm::mat4 rr = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        return glm::lookAt(position(), position() + direction(), glm::vec3(rr * glm::vec4(direction(), 1.0f)));
    }

    glm::mat4 CalcProj() const
    {
        switch (type_)
        {
        case LightType::DIRECTIONAL:
        case LightType::POINT:
        case LightType::SPOT:
            return glm::perspective(glm::radians(120.0f), 1.0f, 0.5f, falloff_end_ * 2.0f);
        default:
            return glm::mat4(1.0f);
        }
    }

    glm::vec3 position() const;
    glm::vec3 direction() const;

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

    bool use_shadow() const
    {
        return use_shadow_;
    }

    void set_use_shadow(bool b)
    {
        use_shadow_ = b;
        if (!b)
        {
            depthmap_ = nullptr;
            return;
        }
        switch (type_)
        {
        case LightType::DIRECTIONAL:
        case LightType::SPOT:
            depthmap_ = DepthMap::Create(1280, 1280);
            break;
        case LightType::POINT:
            depthmap_ = DepthMap::Create(1280, 1280, 1280);
            break;
        default:
            break;
        }
    }

    DepthMap *depth_map()
    {
        return depthmap_.get();
    }

    uint32_t shadow_id()
    {
        return shadow_id_;
    }

    void set_shadow_id(uint32_t id)
    {
        shadow_id_ = id;
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
    DepthMapPtr depthmap_{nullptr};
    uint32_t shadow_id_{0};
};

#endif
