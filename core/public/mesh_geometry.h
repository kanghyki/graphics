#ifndef INCLUDED_MESH_GEOMETRY_H
#define INCLUDED_MESH_GEOMETRY_H

#include "libmath.h"
#include "vertex.h"
#include <utility>
#include <vector>

namespace MeshGeometry
{
struct Data
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

Data GenerateBox();
Data GenerateSphere(uint32_t slice, uint32_t stack);
Data GeneratePlane();
}; // namespace MeshGeometry

#endif
