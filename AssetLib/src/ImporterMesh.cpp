#include "ImporterMesh.h"

#include "Core.h"
#include "BinaryIn.h"

namespace AssetLib
{
    auto ImporterMesh::Import(const std::string& filename) -> Mesh
    {
        Serialisation::BinaryIn in(filename);

        /* - Header - */

        // Signature
        std::string signature(3, ' ');
        signature[0] = in.Read<char>();
        signature[1] = in.Read<char>();
        signature[2] = in.Read<char>();

        if (strcmp(signature.c_str(), "RES") != 0)
        {
            // Error
            return {};
        }

        // Version
        auto version = in.Read<uint8_t>();
        if (version > FORMAT_VERSION)
        {
            // Trying to import format that is newer than supported
            return {};
        }

        /* - Vertices - */

        auto vertexCount = in.Read<uint32_t>();
        std::vector<Vertex> vertices(vertexCount);
        for (size_t i = 0; i < vertexCount; i++)
        {
            auto& vertex = vertices[i];

            vertex.position.x = in.Read<float>();
            vertex.position.y = in.Read<float>();
            vertex.position.z = in.Read<float>();

            vertex.normal.x = in.Read<float>();
            vertex.normal.y = in.Read<float>();
            vertex.normal.z = in.Read<float>();

            vertex.texCoord.x = in.Read<float>();
            vertex.texCoord.y = in.Read<float>();

            vertex.color.x = in.Read<float>();
            vertex.color.y = in.Read<float>();
            vertex.color.z = in.Read<float>();
        }

        /* - Indices - */

        auto indexCount = in.Read<uint32_t>();
        std::vector<uint32_t> indices(indexCount);
        for (size_t i = 0; i < indexCount; i++)
        {
            indices[i] = in.Read<uint32_t>();
        }

        /* - Submeshes - */

        auto submeshCount = in.Read<uint32_t>();
        std::vector<Submesh> submeshes(submeshCount);
        for (size_t i = 0; i < submeshCount; i++)
        {
            auto& submesh = submeshes[i];

            submesh.BaseIndex = in.Read<uint32_t>();
            submesh.BaseVertex = in.Read<uint32_t>();
            submesh.MaterialIndex = in.Read<uint8_t>();
            submesh.IndexCount = in.Read<uint32_t>();
            submesh.VertexCount = in.Read<uint32_t>();
        }

        return { vertices, indices, submeshes };
    }

}  // namespace AssetLib
