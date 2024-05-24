#version 430 core
out vec4 fragColor;
in vec3 texCoord;

uniform samplerCube cubemap;

void main() {
    fragColor = texture(cubemap, texCoord);
}