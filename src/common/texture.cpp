#include "common/texture.hpp"

#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>

Texture::Texture(unsigned int id, std::string type)
    : m_id(id), m_type(std::move(type)), m_filename()
{}

void Texture::FromFile(const std::string& filename, const std::string& directory, bool bGamma)
{
    m_filename = filename;
    std::string path{ directory + '/' + filename };

    glGenTextures(1, &m_id);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        switch (nrComponents)
        {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4:
            default:
                format = GL_RGBA; break;
        }

        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }
    stbi_image_free(data);
}

unsigned int Texture::GetId() const
{
    return m_id;
}

void Texture::SetId(unsigned  int id)
{
    m_id = id;
}

const std::string& Texture::GetType() const
{
    return m_type;
}

void Texture::SetType(std::string type)
{
    m_type = std::move(type);
}

[[nodiscard]] const std::string& Texture::GetFilename() const
{
    return m_filename;
}

void Texture::SetFilename(std::string filename)
{
    m_filename = std::move(filename);
}
