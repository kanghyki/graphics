#include "monster.h"
#include <spdlog/spdlog.h>

Monster ::Monster() : speed_(1.0f)
{
}

Monster ::~Monster()
{
}

void Monster::Update()
{
    glm::vec3 cur_pos = transform_.position();

    transform_.set_position(cur_pos + 1.0f * speed_);
}

void Monster::Render()
{
    SPDLOG_INFO("Monster! {} {} {}", transform_.position().x, transform_.position().y, transform_.position().z);
}
