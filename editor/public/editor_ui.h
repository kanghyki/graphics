#ifndef INCLUDED_EDITOR_UI_H
#define INCLUDED_EDITOR_UI_H

#include "imgui_override.tpp"
#include <imgui.h>
#include <memory>
#include <vector>

class EditorUI
{
  public:
    EditorUI();
    virtual ~EditorUI();

    void Render()
    {
        Update();
        RenderImpl();
        for (const auto &child : children_)
        {
            child->Render();
        }
    };
    void AddUI(std::shared_ptr<EditorUI> ui)
    {
        children_.emplace_back(std::move(ui));
    }

  protected:
    virtual void Update()
    {
    }
    virtual void RenderImpl()
    {
    }

  private:
    EditorUI(const EditorUI &rhs);
    EditorUI &operator=(const EditorUI &rhs);

    std::vector<std::shared_ptr<EditorUI>> children_;
};

#endif