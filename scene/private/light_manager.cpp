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
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightsUniform, l_lights), sizeof(LightUniform) * lights_.count,
                    &lights_.l_lights);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    lights_.count = 0;
}

void LightManager::AddLight(const LightUniform &light)
{
    if (lights_.count >= LightsUniform::LIGHT_MAX)
    {
        SPDLOG_WARN("MAXIMUM LIGHT");
        return;
    }
    lights_.l_lights[lights_.count] = light;
    ++lights_.count;
}