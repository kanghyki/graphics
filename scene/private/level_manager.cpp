#include "level_manager.h"
#include <algorithm>

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
    for (const auto &level : all_level_)
    {
        delete level;
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
    all_level_.push_back(level);
}

void LevelManager::RemoveLevel(const std::string &name)
{
    auto it = std::find_if(all_level_.begin(), all_level_.end(), [&name](Level *level) {
        if (level->name() == name)
        {
            return true;
        }
        return false;
    });
    if (it != all_level_.end())
    {
        all_level_.erase(it);
        delete *it;
    }
}

Level *LevelManager::FindLevel(const std::string &name)
{
    auto it = std::find_if(all_level_.begin(), all_level_.end(), [&name](Level *level) {
        if (level->name() == name)
        {
            return true;
        }
        return false;
    });
    if (it != all_level_.end())
    {
        return *it;
    }
    return nullptr;
}

void LevelManager::SetCurrentLevel(const std::string &name)
{
    cur_level_ = FindLevel(name);
}

Level *LevelManager::GetCurrentLevel()
{
    return cur_level_;
}

std::vector<Level *> LevelManager::GetAllLevel()
{
    return all_level_;
}