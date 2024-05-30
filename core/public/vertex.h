#ifndef INCLUDED_VERTEX_H
#define INCLUDED_VERTEX_H

#include "libmath.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent;
};

#endif