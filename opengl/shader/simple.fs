#version 430 core
out vec4 fragColor;

#include "include/default.incl"
uniform vec4 color;

void main() {
    fragColor = color;
}
