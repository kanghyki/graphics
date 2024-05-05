#include "monster.h"
#include "player.h"
#include "scene_start.h"

SceneStart::SceneStart()
{
}

SceneStart::~SceneStart()
{
}

void SceneStart::Setup()
{
    Object *obj = new Player();
    AddObject(Layer::DEFAULT, obj);

    Object *monster = new Monster();
    AddObject(Layer::MONSTER, monster);
}

void SceneStart::Teardown()
{
}
