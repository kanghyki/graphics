#version 430 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gEmissive;

in float height;
in vec4 normal;
in vec4 position;

void main()
{
    gPosition = position;
    gNormal = vec4(normalize(normal.xyz), 1.0);
	float h = (height + 16) / 64.0f;
    gAlbedoSpec = vec4(h, h, h, 1.0);
    gEmissive = vec4(0.0, 0.0, 0.0, 0.0);
}