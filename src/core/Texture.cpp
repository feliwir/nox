#include "Texture.hpp"

#include <GL/glew.h>

nox::core::Texture::Texture()

{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

nox::core::Texture::~Texture()
{
    glDeleteTextures(1, &m_handle);
    m_handle = 0;
}

void nox::core::Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_handle);
}
