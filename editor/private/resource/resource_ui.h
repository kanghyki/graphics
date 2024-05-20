#ifndef INCLUDED_RESOURCE_UI_H
#define INCLUDED_RESOURCE_UI_H

#include "editor_ui.h"
#include "model.h"
#include "resource_ui_observer.h"
#include "texture.h"

class ResourceUI : public EditorUI, public ResourceUIObservable
{
  private:
    void RenderImpl() override;

    Texture2dPtr texture2d_selected{nullptr};
    ModelPtr model_selected{nullptr};
};

#endif