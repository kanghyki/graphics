#ifndef INCLUDED_EDITOR_UI_H
#define INCLUDED_EDITOR_UI_H

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

    virtual void Update()
    {
    }
    virtual void RenderImpl()
    {
    }
    void AddUI(std::unique_ptr<EditorUI> ui)
    {
        children_.emplace_back(std::move(ui));
    }

  private:
    EditorUI(const EditorUI &rhs);
    EditorUI &operator=(const EditorUI &rhs);

    std::vector<std::unique_ptr<EditorUI>> children_;
};

#endif