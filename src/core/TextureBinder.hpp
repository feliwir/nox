#pragma once
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

namespace nox::core
{
	class TextureBinder
	{
	public:
		static int GetGLTexture(AVHWFramesContext* hwFramesContext);
	};
}