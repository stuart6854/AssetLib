#pragma once

#include "Mesh.h"

#include <string>

namespace AssetLib
{
    class ExporterMesh
    {
    public:
        void Export(const Mesh& mesh, const std::string& filename);

    private:
    };

}  // namespace AssetLib
