#ifndef INCLUDED_SCENE_UI_H
#define INCLUDED_SCENE_UI_H

#include "editor_ui.h"
#include "mesh.h"
#include "model.h"
#include "resource_ui_observer.h"
#include "texture.h"

class Actor;
class Level;
class Layer;
class Camera;
class Transform;
class Light;
class Mesh;
class SceneUI : public EditorUI, public ResourceUIObserver
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
    void ModelDetail();
    void MeshTextureDetail(MeshPtr mesh, TextureType type);

    Level *level_current{nullptr};
    Layer *layer_selected{nullptr};
    Actor *actor_selected{nullptr};
    Texture2dPtr texture2d_selected{nullptr};
    ModelPtr model_selected{nullptr};

  private:
    void UpdateTexture2d(Texture2dPtr tex) override
    {
        texture2d_selected = tex;
    }
    void UpdateModel(ModelPtr model) override
    {
        model_selected = model;
    }
};

#endif