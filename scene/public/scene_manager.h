#ifndef INCLUDED_SCENE_MANAGER_H
#define INCLUDED_SCENE_MANAGER_H

enum class SceneType
{
    EDITOR,
    START_MENU,
    IN_GAME_SCENE_00,
    END,
};

class Scene;

class SceneManager
{
  public:
    static SceneManager *GetInstance();
    ~SceneManager();

    void Init();
    void Update();
    void Render();

    Scene *current_scene()
    {
        return cur_scene_;
    }

  protected:
  private:
    SceneManager();
    SceneManager(const SceneManager &c);
    SceneManager &operator=(const SceneManager &c);

    static SceneManager *instance_;
    Scene *all_scene_[static_cast<int>(SceneType::END)];
    Scene *cur_scene_;
};

#endif
