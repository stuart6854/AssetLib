#pragma once
#include "Vertex.h"

#include <vector>

namespace AssetLib
{
    struct Submesh
    {
        uint32_t BaseVertex;
        uint32_t BaseIndex;
        uint8_t MaterialIndex;
        uint32_t VertexCount;
        uint32_t IndexCount;
    };

    struct Mesh
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Submesh> submeshes;
    };

}  // namespace AssetLib
