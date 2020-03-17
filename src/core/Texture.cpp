#include "Texture.hpp"

#include <GL/glew.h>

nox::core::Texture::Texture()
    : m_handle(0)
{
    glGenTextures(1, &m_handle);
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
