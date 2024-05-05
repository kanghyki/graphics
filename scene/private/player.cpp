#include "input_manager.h"
#include "player.h"
#include <spdlog/spdlog.h>

Player ::Player()
{
}

Player ::~Player()
{
}

void Player::Update()
{
    float fDt = 1.0f;
    glm::vec3 t = transform_.position();
    if (KEY_HOLD(Key::W))
    {
        t.y += 1.0f;
        transform_.set_position(t);
    }
    if (KEY_HOLD(Key::A))
    {
        t.x -= 1.0f;
        transform_.set_position(t);
    }
    if (KEY_HOLD(Key::S))
    {
        t.y -= 1.0f;
        transform_.set_position(t);
    }
    if (KEY_HOLD(Key::D))
    {
        t.x += 1.0f;
        transform_.set_position(t);
    }
}

void Player::Render()
{
    SPDLOG_INFO("{} {} {}", transform_.position().x, transform_.position().y, transform_.position().z);
}
