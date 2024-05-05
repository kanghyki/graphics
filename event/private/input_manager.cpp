#include "input_manager.h"
#include "libopengl.h"

constexpr int g_glfw_key_type_map[static_cast<int>(Key::EOL)] = {
    GLFW_KEY_Q,
    GLFW_KEY_W,
    GLFW_KEY_E,
    GLFW_KEY_A,
    GLFW_KEY_S,
    GLFW_KEY_D,
    GLFW_KEY_LEFT_CONTROL,
    GLFW_KEY_LEFT_ALT,
    GLFW_KEY_LEFT_SHIFT,
    GLFW_KEY_SPACE,
    GLFW_KEY_ENTER,
    GLFW_KEY_ESCAPE,
};

InputManager *InputManager::instance_ = nullptr;

InputManager *InputManager::GetInstance()
{
    if (!instance_)
    {
        instance_ = new InputManager();
    }

    return instance_;
}

InputManager ::InputManager() : key_infos_(GLFW_KEY_LAST + 1)
{
}

InputManager ::~InputManager()
{
}

void InputManager::Init()
{
    for (int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        key_infos_[i].state_ = InputState::NONE;
        key_infos_[i].prev_press_ = false;
    }
}
void InputManager::Update()
{
    for (int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        KeyInfo &info = key_infos_[i];
        if (info.state_ == InputState::AWAY && !info.prev_press_)
        {
            info.state_ = InputState::NONE;
        }
        else if (info.state_ == InputState::TAB && info.prev_press_)
        {
            info.state_ = InputState::HOLD;
        }
    }
    glfwPollEvents();
}

void InputManager::UpdateKey(int key, int action)
{
    KeyInfo &info = key_infos_[key];
    if (action == GLFW_PRESS)
    {
        info.state_ = InputState::TAB;
        info.prev_press_ = true;
    }
    else if (action == GLFW_RELEASE)
    {
        info.state_ = InputState::AWAY;
        info.prev_press_ = false;
    }
}

InputState InputManager::GetKeyInputState(Key key)
{
    int glfw_key = g_glfw_key_type_map[static_cast<int>(key)];
    return key_infos_[glfw_key].state_;
}
