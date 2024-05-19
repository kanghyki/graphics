#ifndef INCLUDED_EDITOR_H
#define INCLUDED_EDITOR_H

#include "editor_ui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <memory>
#include <vector>

class Editor
{
  public:
    static Editor *GetInstance();
    ~Editor();

    void Init(void *window);
    void NewFrame();
    void Render();

  private:
    Editor();
    Editor(const Editor &rhs);
    Editor &operator=(const Editor &rhs);

    static Editor *instance_;
    /* ImGui */
    ImGuiContext *imgui_context_{nullptr};

    std::unique_ptr<EditorUI> ui_;
};

#endif