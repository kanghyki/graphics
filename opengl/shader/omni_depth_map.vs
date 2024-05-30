#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

#include "include/default.incl"

uniform mat4 model;

void main()
{
    vec3 v_position = aPos;
    if (material.use_height_map)
    {
        float height = texture(m_height, aTexCoord).r;
        height = height * 2.0 - 1.0;
        v_position = aPos + (height * material.height_scale * aNormal);
    }
    gl_Position = model * vec4(v_position, 1.0);
}  