#include "Texture.h"

#include <stb/stb_image.h>

Texture::Texture(const std::string& filename, GLenum format, GLenum pixelType)
{
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(filename.c_str(), &widthImg, &heightImg, &numColCh, 0);

    if (numColCh == 3)
        format = GL_RGB;
    else if (numColCh == 4)
        format = GL_RGBA;

    glGenTextures(1, &_id);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _id);
}
