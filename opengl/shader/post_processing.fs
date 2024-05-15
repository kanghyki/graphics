#version 330 core
in vec4 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D main_tex;
uniform float gamma;

void main() {
  vec3 pixel = texture(main_tex, texCoord).xyz;
  vec3 result = pow(pixel.rgb, vec3(gamma));

  fragColor = vec4(result, 1.0);
}
