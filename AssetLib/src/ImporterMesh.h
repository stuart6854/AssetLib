#pragma once

#include "Mesh.h"

#include <string>

namespace AssetLib
{
    class ImporterMesh
    {
    public:
        auto Import(const std::string& filename) -> Mesh;
    };

}  // namespace AssetLib
