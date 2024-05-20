#include "level.h"
#include "layer.h"

Level::Level(const std::string &name) : name_(name)
{
    std::fill(layers_.begin(), layers_.end(), nullptr);
}

Level::~Level()
{
    for (const auto &layer : layers_)
    {
        if (layer)
        {
            delete layer;
        }
    }
}

void Level::Tick()
{
    for (const auto &layer : layers_)
    {
        if (layer)
        {
            layer->Tick();
        }
    }
    for (const auto &layer : layers_)
    {
        if (layer)
        {
            layer->FinalTick();
        }
    }
}

std::vector<Actor *> Level::GetActors(uint32_t layer_mask)
{
    size_t size = 0;
    std::vector<Actor *> ret;

    for (const auto &layer : layers_)
    {
        if (layer && layer->mask() & layer_mask)
        {
            const auto &actors = layer->actors();
            size += actors.size();
        }
    }
    ret.reserve(size);
    for (const auto &layer : layers_)
    {
        if (layer && layer->mask() & layer_mask)
        {
            const auto &actors = layer->actors();
            ret.insert(ret.end(), actors.begin(), actors.end());
        }
    }

    return ret;
}

Layer *Level::AddLayer(const std::string &name)
{
    if (name.empty())
    {
        return nullptr;
    }
    auto it = find_if(layers_.begin(), layers_.end(), [&name](Layer *layer) {
        if (!layer)
        {
            return false;
        }
        return layer->name() == name;
    });
    if (it != layers_.end())
    {
        return nullptr;
    }
    for (uint32_t i = 0; i < LayerMax; ++i)
    {
        if (!layers_[i])
        {
            layers_[i] = new Layer(name);
            layers_[i]->set_mask(i);
            return layers_[i];
        }
    }
}

Layer *Level::GetLayer(uint32_t index)
{
    return layers_[index];
}