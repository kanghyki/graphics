#version 430 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

#include "include/default.incl"

void main() {
  gPosition = vec4(position, 1.0);
  gNormal = vec4(normalize(normal), 1.0);
  gAlbedoSpec.rgb = texture(m_diffuse, texCoord).rgb;
  gAlbedoSpec.a = texture(m_specular, texCoord).r;
}