#include "light_component.h"
#include "light_manager.h"
#include "transform_component.h"

LightComponent::LightComponent() : Component(ComponentType::LIGHT)
{
}

LightComponent::~LightComponent()
{
}

void LightComponent::Tick()
{
    LightManager::GetInstance()->AddLight(this);
}

glm::vec3 LightComponent::position() const
{
    return GetTransformComponent()->position();
}

glm::vec3 LightComponent::direction() const
{
    return GetTransformComponent()->CalcRotateMatrix() * glm::vec4(0.0, 0.0f, -1.0f, 0.0f);
}

LightData LightComponent::to_data() const
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
        if (type_ == LightType::SPOT)
        {
            data.shadow_id = shadow_id_;
            data.view = CalcView();
            data.proj = CalcProj();
        }
        else if (type_ == LightType::POINT)
        {
            data.shadow_id = shadow_id_;
            data.far_plane = far_plane();
        }
        else if (type_ == LightType::DIRECTIONAL)
        {
            data.far_plane = camera_far_plane_;
            std::vector<float> shadowCascadeLevels = cascade_level;
            for (int i = 0; i < shadowCascadeLevels.size(); ++i)
            {
                data.cascade_distance[i * 4] = shadowCascadeLevels[i];
            }
            std::vector<glm::mat4> matrices = matrices_;
            for (int i = 0; i < matrices.size(); ++i)
            {
                data.csm_matrix[i] = matrices[i];
            }
        }
    }

    return data;
}
