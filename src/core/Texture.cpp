#include <GL/glew.h>
#include "Texture.hpp"

nox::core::Texture::Texture() : m_handle(0)
{
	glGenTextures(1, &m_handle);
}

nox::core::Texture::~Texture()
{
	glDeleteTextures(1, &m_handle);
	m_handle = 0;
}
