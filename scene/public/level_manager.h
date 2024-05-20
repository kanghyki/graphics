#ifndef INCLUDED_LEVEL_MANAGER_H
#define INCLUDED_LEVEL_MANAGER_H

#include "level.h"
#include <vector>

class LevelManager
{
  public:
    static LevelManager *GetInstance();
    ~LevelManager();

    void Init();
    void Tick();

    Level *AddLevel(const std::string &name);
    void RemoveLevel(const std::string &name);
    Level *FindLevel(const std::string &name);
    void SetCurrentLevel(const std::string &name);
    Level *GetCurrentLevel();
    std::vector<Level *> GetAllLevel();

  protected:
  private:
    LevelManager();
    LevelManager(const LevelManager &c);
    LevelManager &operator=(const LevelManager &c);

    static LevelManager *instance_;
    std::vector<Level *> all_level_;
    Level *cur_level_;
};

#endif
