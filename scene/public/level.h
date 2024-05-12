#ifndef INCLUDED_LEVEL_H
#define INCLUDED_LEVEL_H

#include <string>
#include <unordered_map>

class Layer;
class Level
{
  public:
    Level(const std::string &name);
    virtual ~Level();

    void Tick();
    void AddLayer(Layer *layer);

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
    std::unordered_map<std::string, Layer *> layers_;
};

#endif
