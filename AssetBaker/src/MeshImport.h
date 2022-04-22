#pragma once

#include "Core/Material.h"
#include "Core/Mesh.h"

#include <assimp/scene.h>

#include <memory>
#include <string>

namespace AssetBaker
{
    class ImporterModel
    {
    public:
        auto GetImportTime() const -> float
        {
            return _importTime;
        }
        auto GetVertexCount() const -> int
        {
            return _vertexCount;
        }
        auto GetTriangleCount() const -> int
        {
            return _triangleCount;
        }
        auto GetSubmeshCount() const -> int
        {
            return _submeshCount;
        }

        auto GetMeshes() const -> const std::vector<std::shared_ptr<Mesh>>&
        {
            return _meshes;
        }
        auto GetMaterials() const -> const std::vector<std::shared_ptr<Material>>&
        {
            return _materials;
        }
        // auto GetTextures() const -> const std::vector<std::shared_ptr<Texture>>& { return _textures; }

        bool Import(const std::string& filename);

    private:
        void ProcessMesh(const aiNode* node, const aiScene* scene);
        auto ProcessSubmesh(const aiMesh* mesh, const aiScene* scene) -> AssetLib::Submesh;

        void ProcessMaterials(const aiScene* scene);
        // void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, );

    private:
        std::string _filename;

        float _importTime = 0.0f;
        int _vertexCount = 0;
        int _triangleCount = 0;
        int _submeshCount = 0;

        std::vector<AssetLib::Vertex> _workingVertices;
        std::vector<uint32_t> _workingIndices;

        std::vector<std::shared_ptr<Mesh>> _meshes;
        std::vector<std::shared_ptr<Material>> _materials;
    };

}  // namespace AssetBaker
