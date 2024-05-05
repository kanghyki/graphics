#ifndef INCLUDED_PLAYER_H
#define INCLUDED_PLAYER_H

#include "Transform.h"
#include "object.h"

class Player : public Object
{
  public:
    Player();
    ~Player();

    void Update() override;
    void Render() override;

  private:
    Player(const Player &c);
    Player &operator=(const Player &c);

    Transform transform_;
};

#endif
