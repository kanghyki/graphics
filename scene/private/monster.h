#ifndef INCLUDED_MONSTER_H
#define INCLUDED_MONSTER_H

#include "Transform.h"
#include "object.h"

class Monster : public Object
{
  public:
    explicit Monster();
    ~Monster();

    void Update();
    void Render();

  private:
    Monster(const Monster &c);
    Monster &operator=(const Monster &c);

    Transform transform_;
    float speed_;
};

#endif
