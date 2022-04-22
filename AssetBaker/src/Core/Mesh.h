#pragma once

#include <Vertex.h>
#include <Mesh.h>

#include <glad/glad.h>

#include <string>
#include <vector>

/**
 * A mesh is made up of multiple submeshes. Each submesh using a different material/textures.
 */
class Mesh
{
public:
    ~Mesh();

    auto GetName() const -> const std::string&
    {
        return _name;
    }
    auto GetIndexCount() const -> uint32_t
    {
        return _indices.size();
    }
    auto GetSubmeshes() const -> const std::vector<AssetLib::Submesh>&
    {
        return _submeshes;
    }

    auto GetVertices() const -> const auto&
    {
        return _vertices;
    }

    auto GetIndices() const -> const auto&
    {
        return _indices;
    }

    void SetName(const std::string& name)
    {
        _name = name;
    }

    void SetVertices(const std::vector<AssetLib::Vertex>& vertices);
    void SetTriangles(const std::vector<uint32_t>& triangles);
    void SetSubmeshes(const std::vector<AssetLib::Submesh>& submeshes);

    void Apply();

    void Bind() const;

private:
    std::string _name;

    std::vector<AssetLib::Vertex> _vertices;
    std::vector<uint32_t> _indices;
    std::vector<AssetLib::Submesh> _submeshes;

    GLuint _VAO;
    GLuint _VBO;
    GLuint _EBO;
};
