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

void LightManager::Init()
{
}

void LightManager::Tick()
{
    Renderer::GetInstance()->GetUBO(UBOType::LIGHT)->Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightsUniform, count), sizeof(int), &lights_.count);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightsUniform, l_lights), sizeof(LightData) * lights_.count,
                    &lights_.l_lights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    lights_.count = 0;
    shadow_2d_count_ = 0;
    shadow_3d_count_ = 0;
}

void LightManager::AddLight(LightComponent *light)
{
    if (lights_.count >= LightsUniform::LIGHT_MAX)
    {
        SPDLOG_WARN("MAXIMUM LIGHT");
        return;
    }
    if (light->use_shadow())
    {
        switch (light->type())
        {
        case LightType::DIRECTIONAL:
        case LightType::SPOT:
            if (shadow_2d_count_ >= LightsUniform::SHADOW_2D_MAX)
            {
                SPDLOG_WARN("MAXIMUM 2D SHADOW");
                break;
            }
            light->set_shadow_id(shadow_2d_count_);
            ++shadow_2d_count_;
            break;
        case LightType::POINT:
            if (shadow_3d_count_ >= LightsUniform::SHADOW_CUBE_MAX)
            {
                SPDLOG_WARN("MAXIMUM 3D SHADOW");
                break;
            }
            light->set_shadow_id(shadow_3d_count_);
            ++shadow_3d_count_;
            break;
        default:
            break;
        }
    }
    else
    {
        light->set_shadow_id(-1);
    }
    lights_.l_lights[lights_.count++] = light->ToData();
}