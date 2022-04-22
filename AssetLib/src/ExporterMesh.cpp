#include "ExporterMesh.h"

#include "Core.h"
#include "BinaryOut.h"

namespace AssetLib
{
    void ExporterMesh::Export(const Mesh& mesh, const std::string& filename)
    {
        Serialisation::BinaryOut out(filename);

        /* - Header - */

        // Signature
        out.Write('R');
        out.Write('E');
        out.Write('S');

        // Version
        out.Write(FORMAT_VERSION);

        /* - Vertices - */
        auto& vertices = mesh.vertices;
        uint32_t vertexCount = vertices.size();
        out.Write(vertexCount);
        for (const auto& vertex : vertices)
        {
            out.Write(vertex.position.x);
            out.Write(vertex.position.y);
            out.Write(vertex.position.z);

            out.Write(vertex.normal.x);
            out.Write(vertex.normal.y);
            out.Write(vertex.normal.z);

            out.Write(vertex.texCoord.x);
            out.Write(vertex.texCoord.y);

            out.Write(vertex.color.x);
            out.Write(vertex.color.y);
            out.Write(vertex.color.z);
        }

        /* - Indices - */
        auto& indices = mesh.indices;
        uint32_t indexCount = indices.size();
        out.Write(indexCount);
        for (auto index : indices)
        {
            out.Write(index);
        }

        /* - Submeshes - */
        auto& submeshes = mesh.submeshes;
        uint32_t submeshCount = submeshes.size();
        out.Write(submeshCount);
        for (const auto& submesh : submeshes)
        {
            out.Write(submesh.BaseIndex);
            out.Write(submesh.BaseVertex);
            out.Write(submesh.MaterialIndex);
            out.Write(submesh.IndexCount);
            out.Write(submesh.VertexCount);
        }
    }

}  // namespace AssetLib
