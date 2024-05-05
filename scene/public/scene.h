#ifndef INCLUDED_SCENE_H
#define INCLUDED_SCENE_H

#include <string>
#include <vector>

enum class Layer
{
    DEFAULT,
    PLAYER,
    MONSTER,
    END,
};

class Object;

class Scene
{
  public:
    Scene();
    virtual ~Scene();

    void Update();
    void Render();

    virtual void Setup() = 0;
    virtual void Teardown() = 0;

    std::string name()
    {
        return name_;
    }
    void set_name(const std::string &name)
    {
        name_ = name;
    }

  protected:
    void AddObject(Layer type, Object *obj);

  private:
    Scene(const Scene &c);
    Scene &operator=(const Scene &c);

    std::string name_;
    std::vector<Object *> objects_[static_cast<int>(Layer::END)];
};

#endif
