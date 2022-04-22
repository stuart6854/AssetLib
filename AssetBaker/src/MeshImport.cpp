#include "MeshImport.h"

#include <filesystem>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GLFW/glfw3.h>

namespace AssetBaker
{
    bool ImporterModel::Import(const std::string& filename)
    {
        _filename = filename;

        _importTime = 0;
        _vertexCount = 0;
        _triangleCount = 0;
        _submeshCount = 0;

        _meshes.clear();
        //_textures.clear();

        const double startTime = glfwGetTime();

        Assimp::Importer importer;
        const auto scene = importer.ReadFile(filename, aiProcess_Triangulate);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            return false;
        }

        for (size_t i = 0; i < scene->mRootNode->mNumChildren; i++)
        {
            ProcessMesh(scene->mRootNode->mChildren[i], scene);
        }

        ProcessMaterials(scene);

        _importTime = static_cast<float>(glfwGetTime() - startTime);

        return true;
    }

    void ImporterModel::ProcessMesh(const aiNode* node, const aiScene* scene)
    {
        std::cout << "Mesh: " << node->mName.C_Str() << std::endl;

        _workingVertices.clear();
        _workingIndices.clear();

        std::vector<AssetLib::Submesh> submeshes;

        // Process all the node's meshes (if any)
        for (size_t i = 0; i < node->mNumMeshes; i++)
        {
            const auto* importedMesh = scene->mMeshes[node->mMeshes[i]];

            submeshes.push_back(ProcessSubmesh(importedMesh, scene));
        }

        const auto mesh = std::make_shared<Mesh>();
        mesh->SetVertices(_workingVertices);
        mesh->SetTriangles(_workingIndices);
        mesh->SetSubmeshes(submeshes);
        mesh->Apply();
        _meshes.push_back(mesh);

        _vertexCount += _workingVertices.size();
        _triangleCount += _workingIndices.size() / 3;

        // Then do the same for each of its children
        for (size_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessMesh(node->mChildren[i], scene);
        }
    }

    auto ImporterModel::ProcessSubmesh(const aiMesh* mesh, const aiScene* scene) -> AssetLib::Submesh
    {
        AssetLib::Submesh submesh{};
        submesh.BaseVertex = _workingVertices.size();
        submesh.BaseIndex = _workingIndices.size();
        submesh.VertexCount = mesh->mNumVertices;

        // Vertices
        for (size_t i = 0; i < mesh->mNumVertices; i++)
        {
            AssetLib::Vertex vertex;

            auto vert = mesh->mVertices[i];
            vertex.position = { vert.x, vert.y, vert.z };

            if (mesh->HasNormals())
            {
                auto norm = mesh->mNormals[i];
                vertex.normal = { norm.x, norm.y, norm.z };
            }

            if (mesh->HasTextureCoords(0))
            {
                auto texCoord = mesh->mTextureCoords[0][i];
                vertex.texCoord = { texCoord.x, texCoord.y };
            }

            if (mesh->HasVertexColors(0))
            {
                auto color = mesh->mColors[0][i];
                vertex.color = { color.r, color.g, color.b };
            }

            _workingVertices.push_back(vertex);
        }

        // Indices
        for (size_t i = 0; i < mesh->mNumFaces; i++)
        {
            auto face = mesh->mFaces[i];
            for (size_t x = 0; x < face.mNumIndices; x++)
            {
                _workingIndices.push_back(submesh.BaseVertex + face.mIndices[x]);

                submesh.IndexCount++;
            }
        }

        submesh.MaterialIndex = mesh->mMaterialIndex;

        _submeshCount++;

        return submesh;
    }

    void ImporterModel::ProcessMaterials(const aiScene* scene)
    {
        _materials.clear();

        if (!scene->HasMaterials())
            return;

        for (size_t i = 0; i < scene->mNumMaterials; i++)
        {
            const auto aiMaterial = scene->mMaterials[i];
            const std::string aiMaterialName = aiMaterial->GetName().C_Str();

            auto material = std::make_shared<Material>();

            aiString aiTexPath;
            uint32_t textureCount = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);

            glm::vec3 diffuseColor(0.8f);
            aiColor3D aiColor;
            if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) == AI_SUCCESS)
                diffuseColor = { aiColor.r, aiColor.g, aiColor.b };

            material->SetDiffuseColor(diffuseColor);

            /* Textures */

            const bool hasDiffuseTexture = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS;
            if (hasDiffuseTexture)
            {
                std::filesystem::path path = _filename;
                auto parentPath = path.parent_path();
                parentPath /= std::string(aiTexPath.data);

                std::string texturePath = parentPath.string();
                std::cout << "Diffuse Texture: " << texturePath << std::endl;

                auto texture = std::make_shared<Texture>(texturePath, GL_RGB, GL_UNSIGNED_BYTE);
                material->SetDiffuseTexture(texture);
            }
            else
            {
                // TODO: Assign default blank texture
            }

            _materials.push_back(material);
        }
    }

}  // namespace AssetBaker
