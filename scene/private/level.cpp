#include "layer.h"
#include "level.h"

Level::Level(const std::string &name) : name_(name), layers_()
{
}

Level::~Level()
{
    for (const auto &layer : layers_)
    {
        delete layer.second;
    }
}

void Level::AddLayer(Layer *layer)
{
    layers_.insert({layer->name(), layer});
}

void Level::Tick()
{
    for (const auto &layer : layers_)
    {
        layer.second->Tick();
    }
}
