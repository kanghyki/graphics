#include "actor.h"
#include "layer.h"

Layer::Layer(const std::string &name) : name_(name), actors_(0)
{
}

Layer::~Layer()
{
}

void Layer::Tick()
{
    for (const auto &object : actors_)
    {
        object->Tick();
    }
}

void Layer::AddActor(Actor *actor)
{
    actors_.push_back(actor);
}
