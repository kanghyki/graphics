#ifndef INCLUDED_KEY_MANAGER_H
#define INCLUDED_KEY_MANAGER_H

#include "libmath.h"
#include <vector>

enum class InputState
{
    TAB,
    HOLD,
    AWAY,
    NONE
};

enum class Key
{
    Q,
    W,
    E,
    A,
    S,
    D,
    LEFT_CTRL,
    LEFT_ALT,
    LEFT_SHIFT,
    SPACE,
    ENTER,
    ESC,
    EOL
};

enum class Mouse
{
    LEFT,
    MIDDLE,
    RIGHT,
    EOL
};

struct InputInfo
{
    InputState state_;
    bool prev_press_;
};

class InputManager
{
  public:
    static InputManager *GetInstance();
    ~InputManager();

    void Init();
    void Update();

    void UpdateKey(int key, int action);
    void UpdateMouse(int key, int action);
    void UpdateCursor(double x, double y);

    InputState GetKeyInputState(Key key);
    InputState GetMouseInputState(Mouse mouse);
    glm::vec2 delta_cursor();

  private:
    InputManager();
    InputManager(const InputManager &c);
    InputManager &operator=(const InputManager &c);

    static InputManager *instance_;

    std::vector<InputInfo> key_infos_;
    std::vector<InputInfo> mouse_infos_;
    bool is_cursor_update_{false};
    glm::vec2 cur_cursor_{0.0f, 0.0f};
    glm::vec2 prev_cursor_{0.0f, 0.0f};
};

#define KEY_CHECK(key, state) (InputManager::GetInstance()->GetKeyInputState(key) == state)
#define KEY_TAB(key) KEY_CHECK(key, InputState::TAB)
#define KEY_HOLD(key) KEY_CHECK(key, InputState::HOLD)
#define KEY_AWAY(key) KEY_CHECK(key, InputState::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, InputState::NONE)

#define MOUSE_CHECK(mouse, state) (InputManager::GetInstance()->GetMouseInputState(mouse) == state)
#define MOUSE_TAB(mouse) MOUSE_CHECK(mouse, InputState::TAB)
#define MOUSE_HOLD(mouse) MOUSE_CHECK(mouse, InputState::HOLD)
#define MOUSE_AWAY(mouse) MOUSE_CHECK(mouse, InputState::AWAY)
#define MOUSE_NONE(mouse) MOUSE_CHECK(mouse, InputState::NONE)

#endif
