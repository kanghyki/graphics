#ifndef INCLUDED_LEVEL_H
#define INCLUDED_LEVEL_H

#include <array>
#include <string>
#include <vector>

class Actor;
class Layer;
class Level
{
  public:
    enum
    {
        LayerMax = 32
    };
    Level(const std::string &name);
    virtual ~Level();

    void Tick();
    std::vector<Actor *> GetActors(uint32_t layer_mask = 0xffffffff);

    Layer *AddLayer(const std::string &name);
    Layer *GetLayer(uint32_t index);

    std::string name() const
    {
        return name_;
    }
    void set_name(const std::string &name)
    {
        name_ = name;
    }

  private:
    Level(const Level &c);
    Level &operator=(const Level &c);

    std::string name_;
    std::array<Layer *, LayerMax> layers_{};
};

#endif
