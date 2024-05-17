#version 430 core

#include "include/default.incl"

layout (location = 0) in vec3 aPos;

#define proj t_proj
#define view t_view
#define model t_model

void main() {
  gl_Position = proj * view * model * vec4(aPos, 1.0);
}
