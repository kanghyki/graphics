#include "scene_manager.h"
#include "scene_start.h"

SceneManager *SceneManager::instance_ = nullptr;

SceneManager *SceneManager::GetInstance()
{
    if (!instance_)
    {
        instance_ = new SceneManager();
    }
    return instance_;
}

SceneManager ::SceneManager() : all_scene_{nullptr}, cur_scene_(nullptr)
{
}

SceneManager ::~SceneManager()
{
    for (size_t i = 0; i < static_cast<int>(SceneType::END); ++i)
    {
        if (all_scene_[i])
        {
            delete all_scene_[i];
        }
    }
}

void SceneManager::Init()
{
    all_scene_[static_cast<int>(SceneType::IN_GAME_SCENE_00)] = new SceneStart();
    all_scene_[static_cast<int>(SceneType::IN_GAME_SCENE_00)]->set_name("Start");

    // all_scene_[static_cast<int>(SceneType::TOOL)] = new SceneStart();
    // all_scene_[static_cast<int>(SceneType::STAGE_00)] = new SceneStart();

    cur_scene_ = all_scene_[static_cast<int>(SceneType::IN_GAME_SCENE_00)];
    cur_scene_->Setup();
}

void SceneManager::Update()
{
    cur_scene_->Update();
}

void SceneManager::Render()
{
    cur_scene_->Render();
}
