#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out vec3 position;
out vec3 normal;
out vec2 texCoord;
out vec3 tangent;

#include "include/default.incl"

#define proj t_proj
#define view t_view
#define model t_model

void main() {
    vec3 v_position = aPos;
    if (material.use_height_map)
    {
        float height = texture(m_height, aTexCoord).r;
        height = height * 2.0 - 1.0;
        v_position = aPos + (height * material.height_scale * aNormal);
    }
    gl_Position = proj * view * model * vec4(v_position, 1.0);
    // TODO: inv_model_uniform
    mat4 invModel = transpose(inverse(model));
    normal = (invModel * vec4(aNormal, 0.0)).xyz;
    tangent = (invModel * vec4(aTangent, 0.0)).xyz;
    texCoord = aTexCoord;
    position = (model * vec4(v_position, 1.0)).xyz;
}