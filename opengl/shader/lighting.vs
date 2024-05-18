#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
  vec3 position;
  vec3 normal;
  vec2 texCoord;
} vs_out;

#include "include/default.incl"

#define proj t_proj
#define view t_view
#define model t_model

void main() {
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    vs_out.position = (model * vec4(aPos, 1.0)).xyz;
    vs_out.normal = (transpose(inverse(model)) * vec4(aNormal, 0.0)).xyz;
    vs_out.texCoord = aTexCoord;
}
