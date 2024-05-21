#ifndef INCLUDED_LIGHT_MANAGER_H
#define INCLUDED_LIGHT_MANAGER_H

#include "uniform_struct.h"
#include <array>

class LightComponent;
class LightManager
{
  public:
    static LightManager *GetInstance();
    ~LightManager();

    void Init();
    void Tick();
    void AddLight(const LightUniform &light);

  private:
    LightManager();
    LightManager(const LightManager &);
    LightManager &operator=(const LightManager &);

    static LightManager *instance_;

    LightsUniform lights_;
};

#endif
