#ifndef INCLUDED_MAIN_MENU_H
#define INCLUDED_MAIN_MENU_H

#include "editor_ui.h"

class MainMenu : public EditorUI
{
  private:
    void RenderImpl() override;
};

#endif