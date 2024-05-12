#ifndef INCLUDED_LAYER_H
#define INCLUDED_LAYER_H

#include <string>
#include <vector>

class Actor;
class Layer
{
  public:
    Layer(const std::string &name);
    ~Layer();

    void Tick();
    void AddActor(Actor *actor);

    std::string name() const
    {
        return name_;
    }
    void set_name(const std::string &name)
    {
        name_ = name;
    }

  private:
    Layer(const Layer &);
    Layer &operator=(const Layer &);

    std::string name_;
    std::vector<Actor *> actors_;
};

#endif
