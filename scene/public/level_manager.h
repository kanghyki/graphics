#ifndef INCLUDED_LEVEL_MANAGER_H
#define INCLUDED_LEVEL_MANAGER_H

#include "level.h"
#include <unordered_map>
#include <vector>

class LevelManager
{
  public:
    static LevelManager *GetInstance();
    ~LevelManager();

    void Init();
    void Tick();

    void AddLevel(Level *level);
    void RemoveLevel(const std::string &name);
    Level *FindLevel(const std::string &name);
    void SetCurrentLevel(const std::string &name);

  protected:
  private:
    LevelManager();
    LevelManager(const LevelManager &c);
    LevelManager &operator=(const LevelManager &c);

    static LevelManager *instance_;
    std::unordered_map<std::string, Level *> all_level_;
    Level *cur_level_;
};

#endif
