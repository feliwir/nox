#include "Media.hpp"

nox::core::Media::Media(std::string_view filename)
{
	// Open video file
	if (avformat_open_input(&m_formatCtx, filename.data(), nullptr, nullptr) != 0)
		return; // Couldn't open file
}
