#include "Shader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw errno;
}

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
{
    const std::string vertexCode = get_file_contents(vertexFile.c_str());
    const std::string fragmentCode = get_file_contents(fragmentFile.c_str());

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // Create shader program
    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Bind() const
{
    glUseProgram(_program);
}

void Shader::Set(const std::string& name, bool val) const
{
    const GLint loc = glGetUniformLocation(_program, name.c_str());
    glUniform1i(loc, val);
}

void Shader::Set(const std::string& name, float val) const
{
    const GLint loc = glGetUniformLocation(_program, name.c_str());
    glUniform1f(loc, val);
}

void Shader::Set(const std::string& name, int val) const
{
    const GLint loc = glGetUniformLocation(_program, name.c_str());
    glUniform1i(loc, val);
}

void Shader::Set(const std::string& name, const glm::vec2& val) const
{
    const GLint loc = glGetUniformLocation(_program, name.c_str());
    glUniform2f(loc, val.x, val.y);
}

void Shader::Set(const std::string& name, const glm::vec3& val) const
{
    const GLint loc = glGetUniformLocation(_program, name.c_str());
    glUniform3f(loc, val.x, val.y, val.z);
}

void Shader::Set(const std::string& name, const glm::vec4& val) const
{
    const GLint loc = glGetUniformLocation(_program, name.c_str());
    glUniform4f(loc, val.x, val.y, val.z, val.w);
}

void Shader::Set(const std::string& name, const glm::mat4& val) const
{
    const GLint loc = glGetUniformLocation(_program, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
}
