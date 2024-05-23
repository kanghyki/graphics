#version 430 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec3 tangent;

#include "include/default.incl"

void main() {
  gPosition = vec4(position, 1.0);
  if (material.use_normal_map)
  {
    vec3 TN = normalize(texture(m_normal, texCoord).xyz * 2.0 - 1.0);
    vec3 N = normalize(normal);
    vec3 T = normalize(tangent);
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T, B, N);
    gNormal = vec4(normalize(TBN * TN), 1.0);
  }
  else
  {
    gNormal = vec4(normalize(normal), 1.0);
  }
  if (material.use_albedo_map)
  {
    gAlbedoSpec.rgb = texture(m_albedo, texCoord).rgb;
  }
  else
  {
    gAlbedoSpec.rgb = material.albedo_color.rbg;
  }
  if (material.use_specular_map)
  {
    gAlbedoSpec.a = texture(m_specular, texCoord).r;
  }
  else
  {
    gAlbedoSpec.a = material.specular_alpha;
  }
}