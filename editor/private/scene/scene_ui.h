#ifndef INCLUDED_SCENE_UI_H
#define INCLUDED_SCENE_UI_H

#include "editor_ui.h"

class Actor;
class Level;
class Camera;
class Transform;
class Light;
class Mesh;
class SceneUI : public EditorUI
{
  private:
    void Update() override;
    void RenderImpl() override;

    void Scene();
    void SceneList();
    void ActorList();

    void ActorDetail();
    void CameraDetail();
    void TransformDetail();
    void LightDetail();
    void MeshDetail();

    Level *level_current{nullptr};
    Actor *actor_selected{nullptr};
};

#endif