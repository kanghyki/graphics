#version 430 core
layout (location = 0) in vec3 aPos;

#include "include/default.incl"

uniform mat4 proj;
uniform mat4 view;
#define model t_model

void main() {
  gl_Position = proj * view * model * vec4(aPos, 1.0);
}
