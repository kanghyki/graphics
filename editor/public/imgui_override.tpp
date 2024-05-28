#ifndef INCLUDED_IMGUI_OVERRIDE_H
#define INCLUDED_IMGUI_OVERRIDE_H

#include <imgui.h>

namespace ImGui
{
template <typename T, typename Getter, typename Setter>
void SliderFloat(const char *label, T *t, Getter getter, Setter setter, float min, float max,
                 const char *format = "%.3f")
{
    float current = (t->*getter)();
    float new_value = current;

    ImGui::SliderFloat(label, &new_value, min, max, format);

    if (current != new_value)
    {
        (t->*setter)(new_value);
    }
}

template <typename T, typename Getter, typename Setter>
void SliderFloat3(const char *label, T *t, Getter getter, Setter setter, float min, float max,
                  const char *format = "%.3f")
{
    glm::vec3 current = (t->*getter)();
    glm::vec3 new_value = current;

    ImGui::SliderFloat3(label, glm::value_ptr(new_value), min, max, format);

    if (current != new_value)
    {
        (t->*setter)(new_value);
    }
}

template <typename T, typename Getter, typename Setter>
void ColorEdit3(const char *label, T *t, Getter getter, Setter setter)
{
    glm::vec3 current = (t->*getter)();
    glm::vec3 new_value = current;

    ImGui::ColorEdit3(label, glm::value_ptr(new_value));

    if (current != new_value)
    {
        (t->*setter)(new_value);
    }
}

template <typename C = int, typename T, typename Getter, typename Setter>
void RadioButton(const char *label, T *t, Getter getter, Setter setter, int v_button)
{
    int current = static_cast<int>((t->*getter)());
    int new_value = current;

    ImGui::RadioButton(label, &new_value, v_button);

    if (current != new_value)
    {
        (t->*setter)(static_cast<C>(new_value));
    }
}

} // namespace ImGui

#endif