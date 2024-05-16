#version 330 core
in vec4 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

struct Material {
    sampler2D ambient;
};
uniform Material material;
uniform float gamma;

void main() {
  vec3 pixel = texture(material.ambient, texCoord).xyz;
  vec3 result = pow(pixel.rgb, vec3(gamma));

  fragColor = vec4(result, 1.0);
}
