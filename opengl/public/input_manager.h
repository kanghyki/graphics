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

struct KeyInfo
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
    void UpdateCursor(double x, double y);

    InputState GetKeyInputState(Key key);
    glm::vec2 delta_cursor() const;

  private:
    InputManager();
    InputManager(const InputManager &c);
    InputManager &operator=(const InputManager &c);

    static InputManager *instance_;

    std::vector<KeyInfo> key_infos_;
    bool is_moved_{false};
    glm::vec2 cur_cursor_;
    glm::vec2 prev_cursor_;
};

#define KEY_CHECK(key, state) (InputManager::GetInstance()->GetKeyInputState(key) == state)
#define KEY_TAB(key) KEY_CHECK(key, InputState::TAB)
#define KEY_HOLD(key) KEY_CHECK(key, InputState::HOLD)
#define KEY_AWAY(key) KEY_CHECK(key, InputState::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, InputState::NONE)

#endif
