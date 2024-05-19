#ifndef INCLUDED_RESOURCE_UI_H
#define INCLUDED_RESOURCE_UI_H

#include "editor_ui.h"

class ResourceUI : public EditorUI
{
  private:
    void RenderImpl() override;
};

#endif