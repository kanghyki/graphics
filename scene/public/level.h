#ifndef INCLUDED_LEVEL_H
#define INCLUDED_LEVEL_H

#include <array>
#include <string>
#include <unordered_map>

/* uint32_t bit mask */
enum LayerType
{
    OBJECTS = 0,
    PLAYER,
    WORLD,
    EOL,
};

#define LAYER_MASK_OBJECTS ()

class Actor;
class Layer;
class Level
{
  public:
    Level(const std::string &name);
    virtual ~Level();

    void Tick();
    void AddActor(Actor *actor, LayerType layer);
    std::vector<Actor *> GetActors(uint32_t layer_mask = 0xffffffff);

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
    std::array<Layer *, LayerType::EOL> layers_;
};

#endif
