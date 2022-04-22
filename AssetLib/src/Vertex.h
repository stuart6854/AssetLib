#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace AssetLib
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
        glm::vec3 color;
    };

}  // namespace AssetLib
