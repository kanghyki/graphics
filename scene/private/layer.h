#ifndef INCLUDED_LAYER_H
#define INCLUDED_LAYER_H

#include <string>
#include <vector>

class Actor;
class Layer
{
  public:
    Layer();
    ~Layer();

    void Tick();
    void FinalTick();

    const std::vector<Actor *> &actors() const
    {
        return actors_;
    }
    std::vector<Actor *> actors()
    {
        return actors_;
    }
    void AddActor(Actor *actor);

  private:
    Layer(const Layer &);
    Layer &operator=(const Layer &);

    std::vector<Actor *> actors_;
};

#endif
