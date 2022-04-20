#pragma once

#include <string>

#include "Vertex.h"

#include <glad/glad.h>

#include <vector>


struct Submesh
{
    uint32_t BaseVertex;
    uint32_t BaseIndex;
    uint32_t MaterialIndex;
    uint32_t VertexCount;
    uint32_t IndexCount;
};

/**
 * A mesh can have multiple submesh's (each submesh using a different material).
 */
class Mesh
{
public:
    ~Mesh();

    auto GetName() const -> const std::string& { return _name; }
    auto GetIndexCount() const -> uint32_t { return _indices.size(); }
    auto GetSubmeshes() const -> const std::vector<Submesh>& { return _submeshes; }

    void SetName(const std::string& name) { _name = name; }

    void SetVertices(const std::vector<Vertex>& vertices);
    void SetTriangles(const std::vector<uint32_t>& triangles);
    void SetSubmeshes(const std::vector<Submesh>& submeshes);

    void Apply();

    void Bind() const;

private:
    std::string _name;

    std::vector<Vertex> _vertices;
    std::vector<uint32_t> _indices;
    std::vector<Submesh> _submeshes;

    GLuint _VAO;
    GLuint _VBO;
    GLuint _EBO;
};
