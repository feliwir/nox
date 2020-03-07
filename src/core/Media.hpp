#pragma once
#include <string_view>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

namespace nox::core
{
	class Media
	{
	public:
		Media(std::string_view filename);
	private:
		AVFormatContext* m_formatCtx = nullptr;

	};
}