#include "object.h"
#include "scene.h"

Scene ::Scene()
{
}

Scene ::~Scene()
{
    for (size_t i = 0; i < static_cast<int>(Layer::END); ++i)
    {
        for (size_t j = 0; j < objects_[i].size(); ++j)
        {
            delete objects_[i][j];
        }
    }
}

void Scene::AddObject(Layer type, Object *obj)
{
    objects_[static_cast<int>(type)].push_back(obj);
}

void Scene::Update()
{
    for (size_t i = 0; i < static_cast<int>(Layer::END); ++i)
    {
        for (size_t j = 0; j < objects_[i].size(); ++j)
        {
            objects_[i][j]->Update();
        }
    }
}

void Scene::Render()
{
    for (size_t i = 0; i < static_cast<int>(Layer::END); ++i)
    {
        for (size_t j = 0; j < objects_[i].size(); ++j)
        {
            objects_[i][j]->Render();
        }
    }
}
