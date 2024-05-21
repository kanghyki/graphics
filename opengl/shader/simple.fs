#version 430 core
out vec4 fragColor;

#include "include/default.incl"

#define COLOR m_ambient_color

void main() {
    fragColor = COLOR;
}
