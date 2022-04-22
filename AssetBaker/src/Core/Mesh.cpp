#include "Mesh.h"

#include <iostream>

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}

void Mesh::SetVertices(const std::vector<AssetLib::Vertex>& vertices)
{
    _vertices = vertices;
}

void Mesh::SetTriangles(const std::vector<uint32_t>& triangles)
{
    _indices = triangles;
}

void Mesh::SetSubmeshes(const std::vector<AssetLib::Submesh>& submeshes)
{
    _submeshes = submeshes;
}

void Mesh::Apply()
{
    glCreateVertexArrays(1, &_VAO);
    glCreateBuffers(1, &_VBO);
    glCreateBuffers(1, &_EBO);

    constexpr size_t VERTEX_SIZE = sizeof(AssetLib::Vertex);
    constexpr size_t OFFSET = offsetof(AssetLib::Vertex, texCoord);

    glNamedBufferData(_VBO, _vertices.size() * VERTEX_SIZE, _vertices.data(), GL_STATIC_DRAW);
    glNamedBufferData(_EBO, _indices.size() * sizeof(uint32_t), _indices.data(), GL_STATIC_DRAW);

    // Position
    glEnableVertexArrayAttrib(_VAO, 0);
    glVertexArrayAttribBinding(_VAO, 0, 0);
    glVertexArrayAttribFormat(_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(AssetLib::Vertex, position));

    // Normal
    glEnableVertexArrayAttrib(_VAO, 1);
    glVertexArrayAttribBinding(_VAO, 1, 0);
    glVertexArrayAttribFormat(_VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(AssetLib::Vertex, normal));

    // TexCoord
    glEnableVertexArrayAttrib(_VAO, 2);
    glVertexArrayAttribBinding(_VAO, 2, 0);
    glVertexArrayAttribFormat(_VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(AssetLib::Vertex, texCoord));

    // Color
    glEnableVertexArrayAttrib(_VAO, 3);
    glVertexArrayAttribBinding(_VAO, 3, 0);
    glVertexArrayAttribFormat(_VAO, 3, 3, GL_FLOAT, GL_FALSE, offsetof(AssetLib::Vertex, color));

    glVertexArrayVertexBuffer(_VAO, 0, _VBO, 0, VERTEX_SIZE);
    glVertexArrayElementBuffer(_VAO, _EBO);
}

void Mesh::Bind() const
{
    glBindVertexArray(_VAO);
}
