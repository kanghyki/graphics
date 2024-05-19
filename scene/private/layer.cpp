#include "layer.h"
#include "actor.h"

Layer::Layer(const std::string &name) : name_(name), actors_(0)
{
}

Layer::~Layer()
{
}

void Layer::Tick()
{
    for (const auto &actor : actors_)
    {
        actor->Tick();
    }
}

void Layer::FinalTick()
{
    for (const auto &actor : actors_)
    {
        actor->FinalTick();
    }
}

void Layer::AddActor(Actor *actor)
{
    actors_.push_back(actor);
}
