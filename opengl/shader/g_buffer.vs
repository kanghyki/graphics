#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

#include "include/default.incl"

#define proj t_proj
#define view t_view
#define model t_model

void main() {
  gl_Position = proj * view * model * vec4(aPos, 1.0);
  normal = (transpose(inverse(model)) * vec4(aNormal, 0.0)).xyz;
  texCoord = aTexCoord;
  position = (model * vec4(aPos, 1.0)).xyz;
}