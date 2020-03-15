#pragma once
#include <GL/glew.h>

namespace nox::core
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		inline GLuint GetHandle() const
		{
			return m_handle;
		}
	protected:
		GLuint m_handle;
	};
}