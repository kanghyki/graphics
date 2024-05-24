#ifndef INCLUDED_RESOURCE_UI_OBSERVER_H
#define INCLUDED_RESOURCE_UI_OBSERVER_H

#include "texture.h"
#include <vector>

class ResourceUIObserver
{
  public:
    ResourceUIObserver()
    {
    }
    virtual ~ResourceUIObserver()
    {
    }

    virtual void UpdateTexture(TexturePtr tex) = 0;
    virtual void UpdateModel(ModelPtr model) = 0;

  private:
};

class ResourceUIObservable
{
  public:
    ResourceUIObservable()
    {
    }
    virtual ~ResourceUIObservable()
    {
    }

    void Register(ResourceUIObserver *observer)
    {
        observers_.push_back(observer);
    }

    void NotifyTexture(TexturePtr tex)
    {
        for (const auto &observer : observers_)
        {
            observer->UpdateTexture(tex);
        }
    }

    void NotifyModel(ModelPtr model)
    {
        for (const auto &observer : observers_)
        {
            observer->UpdateModel(model);
        }
    }

  private:
    std::vector<ResourceUIObserver *> observers_;
};

#endif