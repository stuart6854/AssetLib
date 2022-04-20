#pragma once

#include <string>
#include <glad/glad.h>

class Texture
{
public:
    Texture(const std::string& filename, GLenum format, GLenum pixelType);

    void Bind();

private:
    GLuint _id;

};
