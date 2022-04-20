#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 normal = { 0.0f, 0.0f, 0.0f };
    glm::vec2 texCoord = { 0.0f, 0.0f };
    glm::vec3 color = { 0.0f, 0.0f, 0.0f };
};
