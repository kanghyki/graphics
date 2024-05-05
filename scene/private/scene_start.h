#ifndef INCLUDED_SCENE_START_H
#define INCLUDED_SCENE_START_H

#include "scene.h"

class SceneStart : public Scene
{
  public:
    SceneStart();
    ~SceneStart();

    virtual void Setup() override;
    virtual void Teardown() override;

  private:
    SceneStart(const SceneStart &c);
    SceneStart &operator=(const SceneStart &c);
};

#endif
