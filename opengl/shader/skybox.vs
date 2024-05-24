#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 texCoord;

#include "include/default.incl"

#define proj t_proj
#define view t_view
#define model t_model

void main() {
    texCoord = aPos;
    gl_Position = t_proj * t_view * t_model * vec4(aPos, 1.0);
}