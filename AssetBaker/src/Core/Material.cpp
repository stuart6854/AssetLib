#include "Material.h"

void Material::SetShader(const std::shared_ptr<Shader>& shader)
{
    _shader = shader;
}

void Material::SetDiffuseColor(const glm::vec3 diffuseColor)
{
    _diffuseColor = diffuseColor;
}

void Material::SetMetallic(float metallic)
{
    _metallic = metallic;
}

void Material::SetSpecular(float specular)
{
    _specular = specular;
}

void Material::SetDiffuseTexture(const std::shared_ptr<Texture>& texture)
{
    _diffuseTexture = texture;
}

void Material::Bind()
{
    _shader->Bind();
    _shader->Set("diffuseColor", _diffuseColor);
    //_shader->Set("metallic", _metallic);
    //_shader->Set("specular", _specular);

    if (_diffuseTexture != nullptr)
        _diffuseTexture->Bind();
}
