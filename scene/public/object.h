#ifndef INCLUDED_OBJECT_H
#define INCLUDED_OBJECT_H

#include "libmath.h"

// class Component;

class Object
{
  public:
    Object();
    virtual ~Object();

    virtual void Update() = 0;
    virtual void Render();

  private:
    //    Component *com_[12];
    Object(const Object &c);
    Object &operator=(const Object &c);
};

#endif
