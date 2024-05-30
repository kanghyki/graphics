#include "light_manager.h"
#include "light_component.h"
#include "renderer.h"
#include <spdlog/spdlog.h>

LightManager *LightManager::instance_ = nullptr;

LightManager *LightManager::GetInstance()
{
    if (!instance_)
    {
        instance_ = new LightManager();
    }
    return instance_;
}

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
}

void LightManager::UpdateUBO()
{
    lights_.count = (int)lcs_.size();
    for (int i = 0; i < lights_.count && i < LightsUniform::LIGHT_MAX; ++i)
    {
        lights_.l_lights[i] = lcs_[i]->to_data();
    }
    Renderer::GetInstance()->GetUBO(UBOType::LIGHT)->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightsUniform, count), sizeof(int), &lights_.count);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightsUniform, l_lights), sizeof(LightData) * lights_.count,
                    &lights_.l_lights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    lcs_.clear();
    shadow_2d_count_ = 0;
    shadow_3d_count_ = 0;
}

void LightManager::AddLight(LightComponent *light)
{
    lcs_.push_back(light);
    if (!light->use_shadow())
    {
        return;
    }
    if (light->type() == LightType::SPOT)
    {
        if (shadow_2d_count_ >= LightsUniform::SHADOW_2D_MAX)
        {
            SPDLOG_WARN("MAXIMUM 2D SHADOW");
        }
        light->set_shadow_id(shadow_2d_count_);
        ++shadow_2d_count_;
    }
    else if (light->type() == LightType::POINT)
    {
        if (shadow_3d_count_ >= LightsUniform::SHADOW_CUBE_MAX)
        {
            SPDLOG_WARN("MAXIMUM 3D SHADOW");
        }
        light->set_shadow_id(shadow_3d_count_);
        ++shadow_3d_count_;
    }
}