#ifndef INCLUDED_RENDER_UI_H
#define INCLUDED_RENDER_UI_H

#include "editor_ui.h"

class RenderUI : public EditorUI
{
  private:
    void RenderImpl() override;
};

#endif