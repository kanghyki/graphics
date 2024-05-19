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
    void FinalTick();

    std::string name()
    {
        return name_;
    }
    void set_name(const std::string &name)
    {
        name_ = name;
    }
    uint32_t mask()
    {
        return mask_;
    }
    void set_mask(uint32_t index)
    {
        mask_ = 1 << index;
    }

    void AddActor(Actor *actor);
    const std::vector<Actor *> &actors() const
    {
        return actors_;
    }
    std::vector<Actor *> actors()
    {
        return actors_;
    }

  private:
    Layer(const Layer &);
    Layer &operator=(const Layer &);

    std::string name_;
    uint32_t mask_;
    std::vector<Actor *> actors_;
};

#endif
