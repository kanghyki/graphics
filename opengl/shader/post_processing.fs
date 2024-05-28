#version 430 core
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D main_texture;

uniform bool use_gray_scale;
uniform float gamma;

/* HDR */
uniform bool use_exposure;
uniform float exposure;

/* BLOOM */
uniform bool use_bloom;
uniform float bloom_strength;
uniform sampler2D blured_texture;


void main()
{
    vec3 pixel = texture(main_texture, texCoord).xyz;

    if (use_bloom)
    {
        vec3 bloom_pixel = texture(blured_texture, texCoord).xyz;
        pixel += bloom_pixel * bloom_strength;
    }

    if (use_exposure)
    {
        pixel = vec3(1.0) - exp(-pixel * exposure);
    }
    if (use_gray_scale)
    {
        float average = 0.2126 * pixel.r + 0.7152 * pixel.g + 0.0722 * pixel.b;
        pixel = vec3(average, average, average);
        fragColor = vec4(average, average, average, 1.0);
    }
    fragColor = vec4(pow(pixel.rgb, vec3(gamma)), 1.0);
}