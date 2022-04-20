#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>

#include <string>

class Shader
{
public:
    Shader(const std::string& vertexFile, const std::string& fragmentFile);

    void Bind() const;

    void Set(const std::string& name, bool val) const;
    void Set(const std::string& name, float val) const;
    void Set(const std::string& name, int val) const;
    void Set(const std::string& name, const glm::vec2& val) const;
    void Set(const std::string& name, const glm::vec3& val) const;
    void Set(const std::string& name, const glm::vec4& val) const;
    void Set(const std::string& name, const glm::mat4& val) const;

private:
    GLuint _program;

};
