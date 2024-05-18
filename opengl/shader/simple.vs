#version 430 core
layout (location = 0) in vec3 aPos;

#include "include/default.incl"

#define proj t_proj
#define view t_view
#define model t_model

void main() {
  gl_Position = proj * view * model * vec4(aPos, 1.0);
}
