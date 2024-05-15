#include "layer.h"
#include "level.h"

Level::Level(const std::string &name) : name_(name), layers_()
{
    for (auto &layer : layers_)
    {
        layer = new Layer();
    }
}

Level::~Level()
{
    for (const auto &layer : layers_)
    {
        delete layer;
    }
}

void Level::Tick()
{
    for (const auto &layer : layers_)
    {
        layer->Tick();
    }
    for (const auto &layer : layers_)
    {
        layer->FinalTick();
    }
}

void Level::AddActor(Actor *actor, LayerType layer)
{
    layers_[static_cast<int>(layer)]->AddActor(actor);
}

std::vector<Actor *> Level::GetActors(uint32_t layer_mask)
{
    // TODO: add layer mask
    uint32_t size = 0;
    std::vector<Actor *> ret;

    for (const auto &layer : layers_)
    {
        const auto &actors = layer->actors();
        size += actors.size();
    }
    ret.reserve(size);
    for (const auto &layer : layers_)
    {
        const auto &actors = layer->actors();
        ret.insert(ret.end(), actors.begin(), actors.end());
    }

    return ret;
}
