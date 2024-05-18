#ifndef INCLUDED_EDITOR_H
#define INCLUDED_EDITOR_H

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
    ImGuiContext *imgui_context_;
};

#endif