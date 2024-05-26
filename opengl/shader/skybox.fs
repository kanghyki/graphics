#version 430 core
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

in vec3 localPos;

uniform samplerCube cubemap;

void main() {
    fragColor = texture(cubemap, localPos);
    brightColor = vec4(0.0, 0.0, 0.0, 0.0);
}