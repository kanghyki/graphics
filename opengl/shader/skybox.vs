#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 localPos;

#include "include/default.incl"

#define proj t_proj 
#define view t_view

void main() {
  localPos = aPos;
  mat4 rotView = mat4(mat3(view));
  vec4 clipPos = proj * rotView * vec4(localPos, 1.0);
  gl_Position = clipPos.xyww;
}