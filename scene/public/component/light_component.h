#ifndef INCLUDED_LIGHT_COMPONENT_H
#define INCLUDED_LIGHT_COMPONENT_H

#include "component.h"
#include "libmath.h"
#include "transform.h"
#include "uniform_struct.h"

enum class LightType
{
    DIRECTIONAL = 0,
    POINT,
    SPOT
};

class Light
{
  public:
    Light()
    {
    }
    ~Light()
    {
    }

    LightData ToData() const
    {
        LightData data;

        // memset(&data, 0, sizeof(LightData));
        data.type = static_cast<int>(type_);
        data.falloff_start = falloff_start_;
        data.falloff_end = falloff_end_;
        data.spot_power = spot_power_;

        data.position = position();
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

    Transform transform_;
    glm::vec3 strength_{1.0f, 1.0f, 1.0f};
    float spot_power_{1.0f};
    float falloff_start_{0.0f};
    float falloff_end_{10.0f};

    LightType type_{LightType::DIRECTIONAL};
};

class LightComponent : public Component
{
  public:
    LightComponent();
    ~LightComponent();

    Light &light()
    {
        return light_;
    }
    const Light &light() const
    {
        return light_;
    }

  private:
    LightComponent(const LightComponent &);
    LightComponent &operator=(const LightComponent &);

    void Tick() override;

    Light light_;
};

#endif
