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

    LightUniform ToUniform() const
    {
        LightUniform uniform;

        uniform.type = static_cast<int>(type_);
        uniform.position = position();
        uniform.constant = constant_;
        uniform.linear = linear_;
        uniform.quadratic = quadratic_;

        uniform.direction = direction();

        uniform.cutoff = cutoff_;
        uniform.ambient = ambient_;
        uniform.diffuse = diffuse_;
        uniform.specular = specular_;

        return uniform;
    }

    // Point & Spot
    glm::vec3 position() const
    {
        return transform_.position_;
    }
    const float constant_{1.0f};
    const float linear_{0.09f};
    const float quadratic_{0.032f};
    // Directional & Spot
    glm::vec3 direction() const
    {

        return transform_.CalcRotateMatrix() * glm::vec4(direction_, 0.0f);
    }

    // Spot
    glm::vec2 cutoff_{glm::vec2(55.0f, 5.0f)};

    // All
    Transform transform_;
    glm::vec3 ambient_{glm::vec3(1.0f, 1.0f, 1.0f)};
    glm::vec3 diffuse_{glm::vec3(1.0f, 1.0f, 1.0f)};
    glm::vec3 specular_{glm::vec3(1.0f, 1.0f, 1.0f)};
    glm::vec3 direction_{glm::vec3(0.0f, -1.0f, 0.0f)};

    LightType type_{LightType::DIRECTIONAL};
};

class LightComponent : public Component
{
  public:
    LightComponent();
    ~LightComponent();

  private:
    LightComponent(const LightComponent &);
    LightComponent &operator=(const LightComponent &);

    void Tick() override;

    Light light_;
};

#endif
