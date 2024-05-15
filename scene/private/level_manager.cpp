#include "level_manager.h"

LevelManager *LevelManager::instance_ = nullptr;

LevelManager *LevelManager::GetInstance()
{
    if (!instance_)
    {
        instance_ = new LevelManager();
    }
    return instance_;
}

LevelManager ::LevelManager() : all_level_(), cur_level_(nullptr)
{
}

LevelManager ::~LevelManager()
{
    for (const auto &it : all_level_)
    {
        delete it.second;
    }
}

void LevelManager::Init()
{
}

void LevelManager::Tick()
{
    cur_level_->Tick();
}

void LevelManager::AddLevel(Level *level)
{
    all_level_.insert({level->name(), level});
}

void LevelManager::RemoveLevel(const std::string &name)
{
    all_level_.erase(name);
}

Level *LevelManager::FindLevel(const std::string &name)
{
    auto it = all_level_.find(name);
    if (it == all_level_.end())
    {
        return nullptr;
    }
    return it->second;
}

void LevelManager::SetCurrentLevel(const std::string &name)
{
    auto it = all_level_.find(name);
    cur_level_ = it->second;
}

Level *LevelManager::GetCurrentLevel()
{
    return cur_level_;
}
