#pragma once

#include "Shader.h"
#include "Texture.h"

#include <glm/vec3.hpp>

#include <memory>

class Material
{
public:
    auto GetShader() const -> const auto&
    {
        return _shader;
    }
    void SetShader(const std::shared_ptr<Shader>& shader);

    void SetDiffuseColor(const glm::vec3 diffuseColor);
    void SetMetallic(float metallic);
    void SetSpecular(float specular);

    void SetDiffuseTexture(const std::shared_ptr<Texture>& texture);

    void Bind();

private:
    std::shared_ptr<Shader> _shader;

    glm::vec3 _diffuseColor;
    float _metallic;
    float _specular;

    std::shared_ptr<Texture> _diffuseTexture;
};
