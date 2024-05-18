// TODO: 330 -> 430
#version 330 core
in vec2 texCoord;
out vec4 fragColor;

#include "include/default.incl"

#define main_texture m_ambient
uniform float gamma;

void main() {
  vec3 pixel = texture(main_texture, texCoord).xyz;
  vec3 result = pow(pixel.rgb, vec3(gamma));

  fragColor = vec4(result, 1.0);
}
