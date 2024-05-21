#version 430 core
in vec2 texCoord;
out vec4 fragColor;

uniform float gamma;
uniform sampler2D main_texture;
uniform bool is_gray_scale;

void main() {
    vec3 pixel = texture(main_texture, texCoord).xyz;
    vec3 result = pow(pixel.rgb, vec3(gamma));

    if (is_gray_scale == true) {
        float average = 0.2126 * pixel.r + 0.7152 * pixel.g + 0.0722 * pixel.b;
        fragColor = vec4(average, average, average, 1.0);
    }
    else {
        fragColor = vec4(result, 1.0);
    }
}
